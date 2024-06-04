#ifndef ATASK_H
#define ATASK_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QProcess>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class atask; }
QT_END_NAMESPACE

class atask : public QMainWindow
{
    Q_OBJECT


public:
    atask(QWidget *parent = nullptr);
    ~atask();

    void AddRootCatalog();
    void fileParser();

private slots:
    void on_FileTree_clicked(const QModelIndex &index);

    void on_PerformButton_clicked();

private:
    Ui::atask *ui;
    QString file_dir_;
    QProcess process_;
};
#endif // ATASK_H
