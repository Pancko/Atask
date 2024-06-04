#include "atask.h"
#include "ui_atask.h"

atask::atask(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::atask)
{
    ui->setupUi(this);
    setWindowTitle("Ansible tasks");
    ui->PerformButton->setEnabled(0);
    AddRootCatalog();
}

atask::~atask()
{
    delete ui;
}

void atask::AddRootCatalog()
{
    QFileSystemModel* model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    model->setNameFilters({"*.yml"});

    ui->FileTree->setModel(model);
    ui->FileTree->setRootIndex(model->index(QDir::homePath()));
    ui->FileTree->resizeColumnToContents(0);

    QDir current_path = QDir::current();
    ui->FileTree->expand(model->index(current_path.path(), 0));

    while(current_path.cdUp())
        ui->FileTree->expand(model->index(current_path.path(), 0));
}

void atask::fileParser()
{
    ui->DescriptionEdit->clear();
    ui->HostsEdit->clear();
    QFile file(file_dir_);
    QString line;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            line = stream.readLine();
            if(line.contains("hosts:"))
            {
                ui->HostsEdit->setText(ui->HostsEdit->toPlainText()+line+"\n");
            }
            ui->DescriptionEdit->setText(ui->DescriptionEdit->toPlainText()+line+"\n");
        }
    }
    file.close();
}

void atask::on_FileTree_clicked(const QModelIndex& index_)
{
    file_dir_ = index_.sibling(index_.row(), 0).data().toString();
    if(file_dir_.right(4) == ".yml")
    {
        QModelIndex copy_start_index = index_;
        while(copy_start_index.parent().data().toString() != "home")
        {
            copy_start_index = copy_start_index.parent();
            file_dir_.prepend(copy_start_index.data().toString() + '/');
        }
        file_dir_.prepend("/home/");
        ui->FileEdit->clear();
        ui->FileEdit->append(file_dir_);
        fileParser();
        ui->PerformButton->setEnabled(1);
    }
}

void atask::on_PerformButton_clicked()
{
    if (!file_dir_.isEmpty())
    {
        process_.start("ansible-playbook", QStringList() << file_dir_);
        process_.waitForFinished();
        QString errors(process_.readAllStandardError());
        QString output(process_.readAllStandardOutput());
        ui->LogsEdit->setText(errors + output);
        AddRootCatalog();
    }
    else
    {
        ui->LogsEdit->setText("[ERROR]: no such file!");
    }
}
