#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Engine.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_fileSystemModel(new QFileSystemModel),
    m_fullPathNamesModel(new QStringListModel),
    m_fullPathNamesList(new QStringList),
    m_dir(new QDir),
    m_textEdit(new QTextEdit),
    m_textDocument(new QTextDocument)
{
    ui->setupUi(this);

    m_fileSystemModel->setRootPath(QDir::currentPath());
    ui->treeView->setModel(m_fileSystemModel);
    ui->treeView_3->setModel(m_fileSystemModel);

    if(!userOptions->s_saveToDirectory.empty())
        m_fileSystemModel->setRootPath(userOptions->s_saveToDirectory.c_str());
    else
        m_fileSystemModel->setRootPath(QDir::currentPath());
    ui->treeView_4->setModel(m_fileSystemModel);

    loadPluginFiles();
    setUpComboBox_2();

    ui->textEdit_5->setFontItalic(true);
    ui->textEdit_5->textCursor().insertText("<Save MAPPING and HEADER files>");

    ui->textEdit_6->setFontItalic(true);
    ui->textEdit_6->textCursor().insertText("<Path to Plugin (.so/.dll)>");
}

MainWindow::~MainWindow()
{
    delete m_fullPathNamesList;
    delete m_fullPathNamesModel;
    delete m_fileSystemModel;
    delete m_dir;
    delete m_textEdit;
    delete m_textDocument;
    delete ui;

}

void MainWindow::loadPluginFiles()
{

    std::set<std::string> set_pluginDescription;
    g_engine->getFileManager()->loadFile(PLUGIN_INFO_PATH, set_pluginDescription);
    parseFile(set_pluginDescription, m_pluginDescription, m_pluginNameToFile);
std::cout << "mainwindow::loadpluginfile!" << std::endl;

         // ui->comboBox->addItem(tr("--Algorithm--  (View directory)"));
ui->comboBox->addItem("--Algorithm--   (View directory)");

    QStringList filters;
    QList<QFileInfo> fileList;
    QString pluginFileName;
    QString pluginName;
std::cout << "mainwindow::loadplugin1" << std::endl;
    filters << "*.so" << "*.dll";
    m_dir->setPath(DEFAULT_PLUGIN_PATH);
    //m_dir->setNameFilters(filters);
   fileList =  m_dir->entryInfoList(filters);
std::cout << "mainwindow::loadplugin2" << std::endl;
   if(fileList.empty())
       std::cout << "Cannot access file!" << std::endl;
   else
   {

       for (int i = 0; i < fileList.size(); ++i) {
          pluginFileName = fileList[i].fileName();
          ui->listWidget->addItem(pluginFileName);
           pluginName = m_pluginDescription[pluginFileName.toStdString()].first.c_str();
           ui->comboBox->addItem(pluginName);
        }
   }



}

void MainWindow::parseFile(std::set<std::string>& set_input, std::map<std::string, std::pair<std::string,std::string> >& m_output, std::map<std::string, std::pair<std::string,std::string> >& m_output2)
{
        pair<string,string> m_temp;
        pair<string,string> m_temp2;
        std::set<std::string>::iterator s_iter;
        for(s_iter=set_input.begin(); s_iter!=set_input.end(); ++s_iter)
        {

           // m_temp.clear();
           // m_temp2.clear();

std::cout << "mainwindow::parsefile!" << std::endl;

                std::size_t i_found=0,i_found2=0,i_found3=0;
               std:: string s_value,s_value2,s_value3;

                i_found = (*s_iter).find(":");
                i_found2 = (*s_iter).find(":",i_found+1);
                i_found3 = (*s_iter).find(":",i_found2+1);
                if(i_found != std::string::npos) {
                        s_value =  (*s_iter).substr(0, i_found);
                        s_value2 =  (*s_iter).substr(i_found+1, i_found2-i_found-1);
                        s_value3 =  (*s_iter).substr(i_found2+1, i_found3-i_found2-1);

                        m_temp.first = s_value2;
                        m_temp.second = s_value3;
                        m_output.insert(std::pair<std::string, std::pair<string,string> >(s_value, m_temp));

                        m_temp2.first = s_value;
                        m_temp2.second = s_value3;
                        m_output2.insert(std::pair<std::string, std::pair<string,string> >(s_value2, m_temp2));
                }
        }
}

void MainWindow::getfullPathNamesList(std::set<std::string>& set_input)
{
std::cout << "  MainWindow::getfullPathNamesList :size: " << m_fullPathNamesList->size() << std::endl;
    std::string s_input;
    set_input.clear();
    QStringList::const_iterator constIterator;
    for (constIterator = m_fullPathNamesList->constBegin(); constIterator != m_fullPathNamesList->constEnd();
           ++constIterator) {
        s_input = (*constIterator).toStdString();
        std::cout << "  MainWindow::getfullPathNamesList : "<< (*constIterator).toStdString() << std::endl;
        set_input.insert(s_input);
    }

}

void MainWindow::getSaveDirectory(std::string s_input)
{
    s_input = m_saveDirectory.toStdString();

std::cout << "  MainWindow::getSaveDirectory : " << s_input<< std::endl;
}

void MainWindow::on_pushButton_clicked()
{
    //QFileSystemModel *model = new QFileSystemModel;
    //model->setRootPath(QDir::currentPath());
    //ui->treeView->setModel(model);

    QModelIndex indexPath;
    indexPath = ui->treeView->currentIndex();
    QString path;
    path = m_fileSystemModel->filePath(indexPath);
   // ui->lineEdit->setText(path);

    if(!path.isEmpty())
        m_fullPathNamesList->append(path);

    m_fullPathNamesList->removeDuplicates();
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));

    ui->treeView_2->setHeaderHidden(true);
    ui->treeView_2->setModel(m_fullPathNamesModel);


std::cout << "  MainWindow::on_pushButton_clicked :size: " << m_fullPathNamesList->size() << std::endl;

}

void MainWindow::on_pushButton_2_clicked()
{
    QModelIndex indexPath;
    indexPath = ui->treeView_2->currentIndex();
    QString path = m_fullPathNamesModel->data(indexPath,0).toString();
    m_fullPathNamesList->removeAll(path);
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));
    ui->treeView_2->setModel(m_fullPathNamesModel);

}

void MainWindow::on_pushButton_4_clicked()
{
    m_fullPathNamesList->clear();
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));
    ui->treeView_2->setModel(m_fullPathNamesModel);
}

void MainWindow::on_pushButton_5_clicked()
{

    //tree->setRootIndex(model->index(QDir::currentPath()));
        QModelIndex index;
        if(!userOptions->s_saveToDirectory.empty()) {
            index = m_fileSystemModel->index(userOptions->s_saveToDirectory.c_str());
            if(userOptions->s_adminFileName.empty()) {
                userOptions->s_adminFileName = userOptions->s_saveToDirectory;
                    if(!userOptions->s_projectName.empty()) {
                        userOptions->s_adminFileName += "/" + userOptions->s_projectName;
    cout << "MainWindow::on_pushButton_5_clicked :   userOptions->s_adminFileName" << userOptions->s_adminFileName << endl;
                    }
            }

        }
        else
            index = m_fileSystemModel->setRootPath(QDir::currentPath());

        ui->treeView_4->setRootIndex(index);



   // g_engine->getPlugin()->setUpFile();
   // g_engine->getPlugin()->algorithm();
    std::string s_tempPluginFileName;
    s_tempPluginFileName = userOptions->s_pluginFileName;
    if(s_tempPluginFileName.find(".so")!=string::npos || s_tempPluginFileName.find(".dll")!=string::npos)
         g_engine->getPluginManager()->loadPlugin(s_tempPluginFileName);




}

void MainWindow::on_pushButton_6_clicked()
{
    QFileDialog dialog(this);
   //dialog.setFileMode(QFileDialog::Directory);
   // dialog.setOption(QFileDialog::ShowDirsOnly);
   // dialog.setDirectory

   // dialog.getOpenFileName(this, tr("Choose Directory"),
                         //  ".", QString(), 0, QFileDialog::ShowDirsOnly);

    ui->textEdit->textCursor().document()->clear();
    QString dir;
    dir = dialog.getExistingDirectory(this, tr("Open Directory"),
                                                     ".",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    userOptions->s_saveToDirectory = dir.toStdString();
    if(userOptions->s_projectName.empty())
        userOptions->s_projectName = "ProjectName";
    QString projectName;
    projectName = tr("/") + userOptions->s_projectName.c_str();
    dir.append(projectName);
    ui->textEdit->textCursor().insertText(dir);
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    std::string s_pluginName;
    QString s_pluginFileName;
    std::string s_pluginDescription = "";
   //std::map<string, map<string,string> >::iterator m_iter;
    pair<string,string> pair_pluginInfo;

    s_pluginName = arg1.toStdString();
    pair_pluginInfo = m_pluginNameToFile[s_pluginName];
    s_pluginFileName = pair_pluginInfo.first.c_str();
    s_pluginDescription = pair_pluginInfo.second;
/*
    for(m_iter=m_pluginDescription; m_iter!=m_pluginDescription.end(); ++m_iter)
        map_pluginInfo = *m_iter;
    if(map_pluginInfo.)

*/

   // s_pluginDescription = m_pluginDescription[s_pluginName];
    //QTextDocument *textDocument = new QTextDocument;
    if(s_pluginDescription != "") {
        m_textDocument->setPlainText(s_pluginDescription.c_str());
        s_pluginDescription="";
        m_textEdit->setDocument(m_textDocument);
        //m_textEdit->setDocumentTitle(tr("Plugin Description"));
        ui->stackedWidget->insertWidget(ui->comboBox->currentIndex(),m_textEdit);
    }
    if(s_pluginFileName.endsWith(".so") || s_pluginFileName.endsWith(".dll"))
        userOptions->s_pluginFileName = DEFAULT_PLUGIN_PATH + s_pluginFileName.toStdString();
    connect(ui->comboBox, SIGNAL(activated(int)),
                 ui->stackedWidget, SLOT(setCurrentIndex(int)));

}

void MainWindow::on_treeView_3_clicked(const QModelIndex &index)
{
    //QModelIndex indexPath;
    //indexPath = ui->treeView->currentIndex();
    QString path;
    string s_buffer;
    path = m_fileSystemModel->filePath(index);

    if(!path.isEmpty() && path.endsWith(tr(".cpp")) || path.endsWith(tr(".h")) || path.endsWith(tr(".c")) || path.endsWith(tr(".hpp")) || path.endsWith(tr(".txt"))) {
       // s_path = path.toStdString();
        g_engine->getFileManager()->loadFile(path.toStdString(), s_buffer);
    }

    m_textDocument->setPlainText(s_buffer.c_str());

    ui->textEdit_2->setDocument(m_textDocument);
   // ui->textEdit_3->setModel(m_fullPathNamesModel);
}

void MainWindow::on_treeView_4_clicked(const QModelIndex &index)
{
    QString path;
    string s_buffer;
    path = m_fileSystemModel->filePath(index);

    if(!path.isEmpty() && path.endsWith(tr(".cpp")) || path.endsWith(".h") || path.endsWith(".c") || path.endsWith(".hpp") || path.endsWith(tr(".txt"))) {
       // s_path = path.toStdString();
        g_engine->getFileManager()->loadFile(path.toStdString(), s_buffer);
    }

    m_textDocument->setPlainText(s_buffer.c_str());

    ui->textEdit_3->setDocument(m_textDocument);

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    userOptions->s_projectName = arg1.toStdString();
    QString projectName,dir;
    projectName = tr("/") + userOptions->s_projectName.c_str();
    dir = userOptions->s_saveToDirectory.c_str();
    dir.append(projectName);
    if(!userOptions->s_saveToDirectory.empty()) {
        ui->textEdit->textCursor().document()->clear();
        ui->textEdit->textCursor().insertText(dir);
    }

    dir = userOptions->s_adminFileName.c_str();
    dir.append(projectName);
    dir.append(ADMIN_FILE_NAME);
    if(!userOptions->s_adminFileName.empty()) {
        ui->textEdit_5->textCursor().document()->clear();
        ui->textEdit_5->textCursor().insertText(dir);
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    QFileDialog dialog(this);
   //dialog.setFileMode(QFileDialog::Directory);
   // dialog.setOption(QFileDialog::ShowDirsOnly);
   // dialog.setDirectory

   // dialog.getOpenFileName(this, tr("Choose Directory"),
                         //  ".", QString(), 0, QFileDialog::ShowDirsOnly);

    ui->textEdit_5->textCursor().document()->clear();
    QString dir;
    dir = dialog.getExistingDirectory(this, tr("Open Directory"),
                                                     ".",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    userOptions->s_adminFileName = dir.toStdString();
    //if(userOptions->s_projectName.empty())
        //userOptions->s_projectName = "ProjectName";
    QString projectName;
    projectName = tr("/") + userOptions->s_projectName.c_str();
    dir.append(projectName);
    dir.append(ADMIN_FILE_NAME);
    ui->textEdit_5->textCursor().insertText(dir);

}

void MainWindow::on_pushButton_9_clicked()
{
    ui->textEdit_5->textCursor().document()->clear();
    QFileDialog dialog(this);

    ui->textEdit_5->textCursor().document()->clear();
    QString fileName;
    fileName =  dialog.getOpenFileName(this, tr("Open File"),
                                        "/home",
                                        tr("Plugin (*.so *.dll)"));
    userOptions->s_newPluginPathName = fileName.toStdString();
    ui->textEdit_6->textCursor().insertText(fileName);
}

void MainWindow::on_pushButton_8_clicked()
{
    QString pluginPathName, pluginFileName, plugininName, pluginDescription, pluginCopyToPath;
   // int i_pos;
    pluginPathName =  userOptions->s_newPluginPathName.c_str();



   // i_pos = pluginPathName.lastIndexOf("/");
    pluginFileName = pluginPathName.section("/", -1, -1);
    if(pluginFileName.endsWith(".so") || pluginPathName.endsWith(".dll")) {
        plugininName = ui->lineEdit_3->displayText();
        pluginDescription = ui->textEdit_7->toPlainText();

        pluginCopyToPath = DEFAULT_PLUGIN_PATH + pluginFileName;
        if(!QFile::copy(pluginPathName, pluginCopyToPath))
              cout << "Error copying plugin!" << endl;

        QFile file(PLUGIN_INFO_PATH);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
                return;


            QTextStream out(&file);
            out << pluginFileName << ":" << plugininName << ":" << pluginDescription << ":" << "\n";
    }


}

void MainWindow::setUpComboBox_2()
{
    ui->comboBox_2->addItem("Not to list");
    ui->comboBox_2->addItem("Plugin list");

}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    connect(ui->comboBox_2, SIGNAL(activated(int)),
                 ui->stackedWidget_2, SLOT(setCurrentIndex(int)));
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString pluginFileName, pluginName, pluginDescription;
    pluginFileName = item->text();

    pluginName = m_pluginDescription[pluginFileName.toStdString()].first.c_str();
    pluginDescription = m_pluginDescription[pluginFileName.toStdString()].second.c_str();
    ui->lineEdit_3->setText(pluginName);
    ui->textEdit_7->setText(pluginDescription);
    ui->textEdit_6->setText(pluginFileName);
}
