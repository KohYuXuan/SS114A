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

    createActions();
    createMenus();


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
    loadListOfNames();
    loadListOfKeyWords();

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

void MainWindow::updateListOfWords(const QString& fileName, const QString& s_buffer)
{
    QMessageBox msgBox;
    QString errMsg;

    errMsg.append("Error opening ");
    errMsg.append(fileName);
    errMsg.append(" file!");

    QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly  | QIODevice::Text | QIODevice::Truncate)){
            msgBox.setText(errMsg);
            msgBox.exec();
            return;
        }

    QTextStream out(&file);
    out << s_buffer;

    file.close();

}

void MainWindow::loadListOfKeyWords()
{
    QMessageBox msgBox;
   // QString fileName;

    //fileName = "Config/notToWords.txt";
    QFile file(KEYWORDS_LIST);
        if (!file.open(QIODevice::ReadOnly)){
            msgBox.setText("Error opening configuration file!");
            msgBox.exec();
            return;
        }

    QTextStream in(&file);
    QString in_buffer;
    in_buffer = in.readAll();

    file.close();
    QTextCursor textCursor;
    textCursor = ui->textEdit_4->textCursor();
    textCursor.insertText(in_buffer);
    ui->textEdit_4->setTextCursor(textCursor);

}

void MainWindow::loadListOfNames()
{
    QMessageBox msgBox;
   // QString fileName;

    //fileName = "Config/notToWords.txt";
    QFile file(NOT_TO_LIST);
        if (!file.open(QIODevice::ReadOnly)){
            msgBox.setText("Error opening configuration file!");
            msgBox.exec();
            return;
        }

    QTextStream in(&file);
    QString in_buffer;
    in_buffer = in.readAll();

    file.close();
    QTextCursor textCursor;
    textCursor = ui->textEdit_8->textCursor();
    textCursor.insertText(in_buffer);
    ui->textEdit_8->setTextCursor(textCursor);
}

void MainWindow::loadPluginFiles()
{

    std::set<std::string> set_pluginDescription;
    g_engine->getFileManager()->loadFile(PLUGIN_INFO_PATH, set_pluginDescription);
    parseFile(set_pluginDescription, m_pluginDescription, m_pluginNameToFile);
std::cout << "mainwindow::loadpluginfile!" << std::endl;
ui->comboBox->clear();
ui->listWidget->clear();
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
        m_output.clear();
        m_output2.clear();
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
        QMessageBox msgBox;

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
        else{
            msgBox.setText("Please choose a save directory!");
            msgBox.exec();
            return;
        }


            //index = m_fileSystemModel->setRootPath(QDir::currentPath());

       // ui->treeView_4->setRootIndex(index);



   // g_engine->getPlugin()->setUpFile();
   // g_engine->getPlugin()->algorithm();
    std::string s_tempPluginFileName;
    s_tempPluginFileName = userOptions->s_pluginFileName;
    if(s_tempPluginFileName.find(".so")!=string::npos || s_tempPluginFileName.find(".dll")!=string::npos)
         g_engine->getPluginManager()->loadPlugin(s_tempPluginFileName);
    else {
        msgBox.setText("Please choose a plugin!");
        msgBox.exec();
        return;
    }

    msgBox.setText("Done!");
    msgBox.exec();
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
    cout << " MainWindow::on_comboBox_currentIndexChanged : s_pluginFileName" << s_pluginFileName.toStdString() << endl;

    cout << " MainWindow::on_comboBox_currentIndexChanged : s_pluginDescription" << s_pluginDescription << endl;

   // s_pluginDescription = m_pluginDescription[s_pluginName];
    //QTextDocument *textDocument = new QTextDocument;
    if(s_pluginDescription != "") {
        m_textDocument->setPlainText(s_pluginDescription.c_str());
        s_pluginDescription="";
        m_textEdit->setDocument(m_textDocument);
        //m_textEdit->setDocumentTitle(tr("Plugin Description"));
        //ui->stackedWidget->insertWidget(ui->comboBox->currentIndex(),m_textEdit);
        ui->stackedWidget->addWidget(m_textEdit);
        ui->stackedWidget->setCurrentWidget(m_textEdit);
    }
    if(s_pluginFileName.endsWith(".so") || s_pluginFileName.endsWith(".dll"))
        userOptions->s_pluginFileName = DEFAULT_PLUGIN_PATH + s_pluginFileName.toStdString();

cout << " MainWindow::on_comboBox_currentIndexChanged   userOptions->s_pluginFileName : " << userOptions->s_pluginFileName << endl;
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
    //ui->textEdit_5->textCursor().document()->clear();
    QFileDialog dialog(this);

    ui->textEdit_6->textCursor().document()->clear();
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
    QMessageBox msgBox;
    pluginPathName =  userOptions->s_newPluginPathName.c_str();



   // i_pos = pluginPathName.lastIndexOf("/");
    pluginFileName = pluginPathName.section("/", -1, -1);
    if(pluginFileName.endsWith(".so") || pluginPathName.endsWith(".dll")) {
        plugininName = ui->lineEdit_3->displayText();
        pluginDescription = ui->textEdit_7->toPlainText();


        pluginDescription.remove("\n");
        pluginDescription.remove("\r");

        pluginCopyToPath = DEFAULT_PLUGIN_PATH + pluginFileName;
        QFile pluginFile(pluginCopyToPath);
        if(pluginFile.exists()) {
            msgBox.setText("The plugin already existed.");
            msgBox.exec();
            return;
        }

        if(!QFile::copy(pluginPathName, pluginCopyToPath)) {
            msgBox.setText("Error copying plugin!");
            msgBox.exec();
            return;
        }

        QFile file(PLUGIN_INFO_PATH);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
                return;

            QTextStream out(&file);
            out << pluginFileName << ":" << plugininName << ":" << pluginDescription << ":" << "\n";
            setPluginEditFile();
            msgBox.setText("The plugin has been saved.");
            msgBox.exec();
            file.close();
            loadPluginFiles();
    }
    else {
        msgBox.setText("Please choose a plugin!");
        msgBox.exec();
        return;
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
    QString pluginFileName, pluginName, pluginDescription, pluginPathName;
    pluginFileName = item->text();
    pluginPathName = DEFAULT_PLUGIN_PATH + pluginFileName;
    pluginName = m_pluginDescription[pluginFileName.toStdString()].first.c_str();
    pluginDescription = m_pluginDescription[pluginFileName.toStdString()].second.c_str();
    ui->lineEdit_3->setText(pluginName);
    ui->textEdit_7->setText(pluginDescription);
    ui->textEdit_6->setText(pluginPathName);
    userOptions->s_newPluginPathName = pluginPathName.toStdString();

}

void MainWindow::on_pushButton_11_clicked()
{
    QString pluginFileName, pluginName, pluginDescription, pluginPathName;
    QMessageBox msgBox;


        pluginName = ui->lineEdit_3->displayText();
        pluginDescription = ui->textEdit_7->toPlainText();
        pluginPathName = ui->textEdit_6->toPlainText();
        pluginFileName = pluginPathName.section("/", -1, -1);

        pluginDescription.remove("\n");
        pluginDescription.remove("\r");

        QFile pluginFile(pluginPathName);
        if(!pluginFile.exists()) {
            msgBox.setText("The plugin does not exist.");
            msgBox.exec();
            return;
        }
        pluginFile.close();

        QFile file(PLUGIN_INFO_PATH);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Append)){
                msgBox.setText("Error opening configuration file.");
                msgBox.exec();
                return;
            }

        QTextStream out(&file);
        out << pluginFileName << ":" << pluginName << ":" << pluginDescription << ":" << "\n";

        file.close();
        map_editFile.erase(pluginFileName);
        loadPluginEditFile();
        setPluginEditFile();
        msgBox.setText("The plugin configuration file has been modified.");
        msgBox.exec();
        loadPluginFiles();
}
void MainWindow::loadPluginEditFile()
{
    //map<QString,QString> map_editFile;
    //map<QString,QString>::iterator m_iter;
    vector<string> pluginEditFile;
    vector<string>::reverse_iterator s_iter;
    QString readLine, pluginFileName, pluginInfo;
    g_engine->getFileManager()->loadFile(PLUGIN_INFO_PATH, pluginEditFile);
    for(s_iter=pluginEditFile.rbegin(); s_iter!=pluginEditFile.rend(); ++s_iter){
        readLine = (*s_iter).c_str();
        cout << "loadPluginEditFile : readLine" << readLine.toStdString() << endl;
        pluginFileName = readLine.section(":", 0, 0);
        pluginInfo = readLine.section(":", 1);
        map_editFile.insert(pair<QString,QString>(pluginFileName,pluginInfo));
    }
}

void MainWindow::setPluginEditFile()
{
    QMessageBox msgBox;

    //loadPluginEditFile();
    QFile file(PLUGIN_INFO_PATH);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
            msgBox.setText("Error opening plugin configuration file.");
            msgBox.exec();
            return;
        }

    map_editFile.erase("fileName");
    QTextStream out(&file);
    out << "fileName:Name:Description:" << "\n";
    for(m_iter=map_editFile.begin(); m_iter!=map_editFile.end(); ++m_iter){
         out << (*m_iter).first << ":" << (*m_iter).second << "\n";
     }
    file.close();

     //msgBox.setText("The plugin configuration file has been modified.");
     //msgBox.exec();
}

void MainWindow::on_pushButton_12_clicked()
{
        QString pluginFileName, pluginName, pluginDescription, pluginPathName;
        QMessageBox msgBox;

        pluginName = ui->lineEdit_3->displayText();
        pluginDescription = ui->textEdit_7->toPlainText();
        pluginPathName = ui->textEdit_6->toPlainText();
        pluginFileName = pluginPathName.section("/", -1, -1);

        QFile pluginFile(pluginPathName);
        if(!pluginFile.exists()) {
            msgBox.setText("The plugin does not exist.");
            msgBox.exec();
            return;
        }

        msgBox.setText("Do you want to remove plugin?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        msgBox.setStandardButtons(QMessageBox::Ok);
        switch (ret) {
          case QMessageBox::Yes:
                    if(!pluginFile.remove()){
                        msgBox.setText("Error removing plugin!");
                        msgBox.exec();
                        return;
                    }
                    pluginFile.close();
                    loadPluginFiles();

                    map_editFile.erase(pluginFileName);
                    setPluginEditFile();
                    msgBox.setText("Plugin removed!");
                    msgBox.exec();
              break;
          case QMessageBox::No:
              return;
              break;
          default:
              break;
        }

}

void MainWindow::on_pushButton_7_clicked()
{
    QMessageBox msgBox;
    QString out_notToList, out_keyWords;
    out_notToList = ui->textEdit_8->document()->toPlainText();
    out_keyWords = ui->textEdit_4->document()->toPlainText();

cout << "on_pushButton_12_click out_notToList : "<< out_notToList.toStdString() << endl;
cout << "on_pushButton_12_click out_keyWords : "<< out_keyWords.toStdString() << endl;

    QString s_line(" ");
    QStringList s_list;
    QTextStream format(&out_notToList);
    while(s_line != NULL) {
            s_line = format.readLine();
            s_list << s_line;
    }
    s_list.sort();
    out_notToList.clear();
   // out_notToList

    cout << " sort not_to_word====================" << endl;
    for (int i = 0; i < s_list.size(); ++i)
              cout << s_list.at(i).toStdString() << endl;






































    msgBox.setText("Do you want update the lists?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    msgBox.setStandardButtons(QMessageBox::Ok);
    switch (ret) {
      case QMessageBox::Yes:
            updateListOfWords(NOT_TO_LIST,out_notToList);
            updateListOfWords(KEYWORDS_LIST,out_keyWords);
            msgBox.setText("Lists updated!");
            msgBox.exec();
          break;
      case QMessageBox::No:
          break;
      default:
          break;
    }
}

void MainWindow::createActions()
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setStatusTip(tr("Show the application's quit box"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    mainMenu = menuBar()->addMenu(tr("&Menu"));
    mainMenu->addAction(quitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("<b>XenusEngine</b> - Obfuscate Your Source Code.\n"
               "Please visit <a href='www.xenusengine.com'>XenusEngine</a> for more info!"));
}

void MainWindow::quit()
{
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setText("Are you sure you want to Quit?");
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Yes:
             QApplication::exit(0);
          break;
      case QMessageBox::No:
          break;
      default:
          break;
    }


}
