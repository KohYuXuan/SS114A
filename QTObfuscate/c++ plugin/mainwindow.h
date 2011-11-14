#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QString>
#include <string>
#include <set>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void getfullPathNamesList(std::set<std::string>& set_input);
    void getSaveDirectory(std::string s_input);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_fileSystemModel;
    QStringListModel *m_fullPathNamesModel;
    QStringList *m_fullPathNamesList;
    QString m_saveDirectory;
};
#endif // MAINWINDOW_H
