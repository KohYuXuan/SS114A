#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QTextEdit>
#include <string>
#include <set>

#define DEFAULT_PLUGIN_PATH "./plugin/"
#define PLUGIN_INFO_PATH "plugin/plugin.txt"

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
    void loadPluginFiles();
    void parseFile(std::set<std::string>& set_input, std::map<std::string,std::string>& m_output);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_treeView_3_clicked(const QModelIndex &index);

    void on_treeView_4_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_fileSystemModel;
    QStringListModel *m_fullPathNamesModel;
    QStringList *m_fullPathNamesList;
    QString m_saveDirectory;
    QDir *m_dir;
    QTextEdit *m_textEdit;
    QTextDocument *m_textDocument;
    std::map<std::string, std::string> m_pluginDescription;
};
#endif // MAINWINDOW_H
