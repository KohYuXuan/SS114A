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
    m_textDocument(new QTextDocument),
    m_textObfuscated(new QTextDocument),
    m_textOriginal(new QTextDocument)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    m_fileSystemModel->setRootPath(QDir::currentPath());
    ui->tree_chooseFile->setModel(m_fileSystemModel);
    ui->tree_viewOriginal->setModel(m_fileSystemModel);

    if(!userOptions->s_saveToDirectory.empty())
        m_fileSystemModel->setRootPath(userOptions->s_saveToDirectory.c_str());
    else
        m_fileSystemModel->setRootPath(QDir::currentPath());
    ui->tree_viewResult->setModel(m_fileSystemModel);

    loadPluginFiles();
    setUpComboBox_2();
    loadListOfNames();
    loadListOfKeyWords();

    QString s_temp;
    s_temp = "<Save MAPPING and HEADER files>\n";
    s_temp.append(QDir::homePath());
    ui->text_adminPath->setFontItalic(true);
    ui->text_adminPath->textCursor().insertText(s_temp);
    userOptions->s_adminFileName = QDir::homePath().toStdString();

    ui->text_pluginPath->setFontItalic(true);
    ui->text_pluginPath->textCursor().insertText("<Path to Plugin (.so/.dll)>");
}

MainWindow::~MainWindow()
{
    delete m_fullPathNamesList;
    delete m_fullPathNamesModel;
    delete m_fileSystemModel;
    delete m_dir;
    delete m_textEdit;
    delete m_textDocument;
    delete  m_textObfuscated;
    delete m_textOriginal;
    delete ui;

}

bool MainWindow::updateListOfWords(const QString& fileName, const QString& s_buffer)
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
            return false;
        }

    QTextStream out(&file);
    out << s_buffer;

    file.close();

}

void MainWindow::loadListOfKeyWords()
{
    QMessageBox msgBox;

    QFile file(KEYWORDS_LIST);
        if (!file.open(QIODevice::ReadOnly)){
            msgBox.setText("Error opening keywords.txt!");
            msgBox.exec();
            return;
        }

    QTextStream in(&file);
    QString in_buffer;
    in_buffer = in.readAll();

    file.close();
    QTextCursor textCursor;
    textCursor = ui->text_keywords->textCursor();
    textCursor.insertText(in_buffer);
    ui->text_keywords->setTextCursor(textCursor);

}

void MainWindow::loadListOfNames()
{
    QMessageBox msgBox;

    QFile file(NOT_TO_LIST);
        if (!file.open(QIODevice::ReadOnly)){
            msgBox.setText("Error opening notToWords.txt!");
            msgBox.exec();
            return;
        }

    QTextStream in(&file);
    QString in_buffer;
    in_buffer = in.readAll();

    file.close();
    QTextCursor textCursor;
    textCursor = ui->text_notToList->textCursor();
    textCursor.insertText(in_buffer);
    ui->text_notToList->setTextCursor(textCursor);
}

void MainWindow::loadPluginFiles()
{

    std::set<std::string> set_pluginDescription;
    g_engine->getFileManager()->loadFile(PLUGIN_INFO_PATH, set_pluginDescription);
    parseFile(set_pluginDescription, m_pluginDescription, m_pluginNameToFile);
    ui->comboBox->clear();
    ui->listWidget->clear();

    ui->comboBox->addItem("--Choose Algorithm--");

    QStringList filters;
    QList<QFileInfo> fileList;
    QString pluginFileName;
    QString pluginName;

    filters << "*.so" << "*.dll";
    m_dir->setPath(DEFAULT_PLUGIN_PATH);
    fileList =  m_dir->entryInfoList(filters);

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
    std::string s_input;
    set_input.clear();
    QStringList::const_iterator constIterator;
    for (constIterator = m_fullPathNamesList->constBegin(); constIterator != m_fullPathNamesList->constEnd();
           ++constIterator) {
        s_input = (*constIterator).toStdString();
        set_input.insert(s_input);
    }
}

void MainWindow::getSaveDirectory(std::string s_input)
{
    s_input = m_saveDirectory.toStdString();
}

void MainWindow::on_btn_addFiles_clicked()
{
    QModelIndex indexPath;
    indexPath = ui->tree_chooseFile->currentIndex();
    QString path;
    path = m_fileSystemModel->filePath(indexPath);

    if(!path.isEmpty())
        m_fullPathNamesList->append(path);

    m_fullPathNamesList->removeDuplicates();
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));

    ui->tree_selectedFiles->setHeaderHidden(true);
    ui->tree_selectedFiles->setModel(m_fullPathNamesModel);
}

void MainWindow::on_btn_unselectFiles_clicked()
{
    QModelIndex indexPath;
    indexPath = ui->tree_selectedFiles->currentIndex();
    QString path = m_fullPathNamesModel->data(indexPath,0).toString();
    m_fullPathNamesList->removeAll(path);
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));
    ui->tree_selectedFiles->setModel(m_fullPathNamesModel);

}

void MainWindow::on_btn_unselectAll_clicked()
{
    m_fullPathNamesList->clear();
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));
    ui->tree_selectedFiles->setModel(m_fullPathNamesModel);
    ui->tree_selectedFiles->setHeaderHidden (true);
}

void MainWindow::on_btn_start_clicked()
{
        QMessageBox msgBox;

        QModelIndex index;
        if(!userOptions->s_saveToDirectory.empty()) {
            index = m_fileSystemModel->index(userOptions->s_saveToDirectory.c_str());
            if(userOptions->s_adminFileName.empty()) {
                userOptions->s_adminFileName = userOptions->s_saveToDirectory;
                    if(!userOptions->s_projectName.empty()) {
                        userOptions->s_adminFileName += "/" + userOptions->s_projectName;
                    }
            }
        }
        else{
            msgBox.setText("Please choose a save directory!");
            msgBox.exec();
            return;
        }

        // Update the view that display the directory that contains output files
        ui->tree_viewResult->setRootIndex(index);

        std::string s_tempPluginFileName;
        s_tempPluginFileName = userOptions->s_pluginFileName;
        if(s_tempPluginFileName.find(".so")!=string::npos || s_tempPluginFileName.find(".dll")!=string::npos) {
             int err = g_engine->getPluginManager()->loadPlugin(s_tempPluginFileName);
             if(err == 0) {
                 msgBox.setText("Done!");
                 msgBox.exec();
             }
             if(err == CANNOT_LOAD_PLUGIN) {
                 msgBox.setText("Load plugin failed!");
                 msgBox.exec();
             }
             if(err == ALGORITHM_FAILED) {
                 msgBox.setText("Algorithm failed : Please make sure correct file is choosen!");
                 msgBox.exec();
             }
        }
        else {
            msgBox.setText("Please choose a plugin!");
            msgBox.exec();
            return;
        }

        // save configuration to text file
        QString s_output, s_list;

        s_output.setNum( ui->comboBox->currentIndex () );
        s_output.append(";");
        s_output.append( userOptions->s_saveToDirectory.c_str() ) ;
        s_output.append(";");
        s_list =  m_fullPathNamesList->join(";");
        s_output += s_list;
        s_output.append(";");

        g_engine->getFileManager()->writeFile(s_output.toStdString(),CONFIG_FILE);
}

void MainWindow::on_btn_saveTo_clicked()
{
    QFileDialog dialog(this);

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
    pair<string,string> pair_pluginInfo;

    s_pluginName = arg1.toStdString();
    pair_pluginInfo = m_pluginNameToFile[s_pluginName];
    s_pluginFileName = pair_pluginInfo.first.c_str();
    s_pluginDescription = pair_pluginInfo.second;

    if(s_pluginDescription != "") {
        m_textDocument->setPlainText(s_pluginDescription.c_str());
        s_pluginDescription="";
        m_textEdit->setDocument(m_textDocument);
        ui->stackedWidget->addWidget(m_textEdit);
        ui->stackedWidget->setCurrentWidget(m_textEdit);
    }

    if(s_pluginFileName.endsWith(".so") || s_pluginFileName.endsWith(".dll"))
        userOptions->s_pluginFileName = DEFAULT_PLUGIN_PATH + s_pluginFileName.toStdString();

    // update the view that display the plugin desctiption on "Start" tab
    // depending on the selected index of combobox
    connect(ui->comboBox, SIGNAL(activated(int)),
                 ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

void MainWindow::on_tree_viewOriginal_clicked(const QModelIndex &index)
{
    QString path;
    string s_buffer;
    path = m_fileSystemModel->filePath(index);

    if(!path.isEmpty() && path.endsWith(tr(".cpp"))
            || path.endsWith(tr(".h"))
            || path.endsWith(tr(".c"))
            || path.endsWith(tr(".hpp"))
            || path.endsWith(tr(".txt")))
    {

        g_engine->getFileManager()->loadFile(path.toStdString(), s_buffer);
    }

    m_textOriginal->setPlainText(s_buffer.c_str());
    ui->text_viewOriginal->setDocument(m_textOriginal);
}

void MainWindow::on_tree_viewResult_clicked(const QModelIndex &index)
{
    QString path;
    string s_buffer;
    path = m_fileSystemModel->filePath(index);

    if(!path.isEmpty() && path.endsWith(tr(".cpp"))
            || path.endsWith(".h")
            || path.endsWith(".c")
            || path.endsWith(".hpp")
            || path.endsWith(tr(".txt")))
    {
        g_engine->getFileManager()->loadFile(path.toStdString(), s_buffer);
    }

    m_textObfuscated->setPlainText(s_buffer.c_str());
    ui->text_viewResult->setDocument(m_textObfuscated);
}

void MainWindow::on_btn_adminPath_clicked()
{
    QFileDialog dialog(this);

    ui->text_adminPath->textCursor().document()->clear();
    QString dir;
    dir = dialog.getExistingDirectory(this, tr("Open Directory"),
                                                     ".",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    userOptions->s_adminFileName = dir.toStdString();

    QString projectName;
    projectName = tr("/") + userOptions->s_projectName.c_str();
    dir.append(projectName);
    dir.append(ADMIN_FILE_NAME);
    ui->text_adminPath->textCursor().insertText(dir);

}

void MainWindow::on_btn_findPlugin_clicked()
{

    QFileDialog dialog(this);
    QString fileName;

    ui->text_pluginPath->textCursor().document()->clear();
    fileName =  dialog.getOpenFileName(this, tr("Open File"),
                                        "/home",
                                        tr("Plugin (*.so *.dll)"));
    userOptions->s_newPluginPathName = fileName.toStdString();
    ui->text_pluginPath->textCursor().insertText(fileName);
}

void MainWindow::on_btn_savePlugin_clicked()
{
    QString pluginPathName, pluginFileName, plugininName, pluginDescription, pluginCopyToPath;
    QMessageBox msgBox;

    pluginPathName = userOptions->s_newPluginPathName.c_str();
    pluginFileName = pluginPathName.section("/", -1, -1);

    if(pluginFileName.endsWith(".so") || pluginPathName.endsWith(".dll")) {
        plugininName = ui->line_pluginName->displayText();
        pluginDescription = ui->text_pluginDescription->toPlainText();

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
    ui->comboBox_2->addItem("Data Types list");
    ui->comboBox_2->addItem("C++ Keywords List");

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
    ui->line_pluginName->setText(pluginName);
    ui->text_pluginDescription->setText(pluginDescription);
    ui->text_pluginPath->setText(pluginPathName);
    userOptions->s_newPluginPathName = pluginPathName.toStdString();

}

void MainWindow::on_btn_editPlugin_clicked()
{
        QString pluginFileName, pluginName, pluginDescription, pluginPathName;
        QMessageBox msgBox;

        pluginName = ui->line_pluginName->displayText();
        pluginDescription = ui->text_pluginDescription->toPlainText();
        pluginPathName = ui->text_pluginPath->toPlainText();
        //pluginFileName = DEFAULT_PLUGIN_PATH;
        pluginFileName += pluginPathName.section("/", -1, -1);
        pluginPathName = DEFAULT_PLUGIN_PATH + pluginFileName;

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
        out << pluginFileName << ":"
            << pluginName << ":"
            << pluginDescription
            << ":" << "\n";

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
    vector<string> pluginEditFile;
    vector<string>::reverse_iterator s_iter;
    QString readLine, pluginFileName, pluginInfo;

    g_engine->getFileManager()->loadFile(PLUGIN_INFO_PATH, pluginEditFile);

    for(s_iter=pluginEditFile.rbegin(); s_iter!=pluginEditFile.rend(); ++s_iter){
        readLine = (*s_iter).c_str();
        pluginFileName = readLine.section(":", 0, 0);
        pluginInfo = readLine.section(":", 1);
        map_editFile.insert(pair<QString,QString>(pluginFileName,pluginInfo));
    }
}

void MainWindow::setPluginEditFile()
{
    QMessageBox msgBox;
    QFile file(PLUGIN_INFO_PATH);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        msgBox.setText("Error opening plugin configuration file.");
        msgBox.exec();
        return;
    }

    map_editFile.erase("fileName");

    QTextStream out(&file);

    for(m_iter=map_editFile.begin(); m_iter!=map_editFile.end(); ++m_iter){
         out << (*m_iter).first << ":" << (*m_iter).second << "\n";
     }

    file.close();
}

void MainWindow::on_btn_removePlugin_clicked()
{
        QString pluginFileName, pluginName, pluginDescription, pluginPathName;
        QMessageBox msgBox;

        pluginName = ui->line_pluginName->displayText();
        pluginDescription = ui->text_pluginDescription->toPlainText();
        pluginPathName = ui->text_pluginPath->toPlainText();
        //pluginFileName = DEFAULT_PLUGIN_PATH;
        pluginFileName += pluginPathName.section("/", -1, -1);
        pluginPathName = DEFAULT_PLUGIN_PATH + pluginFileName;

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

void MainWindow::on_btn_updateWordsList_clicked()
{
    QMessageBox msgBox;
    QTextCursor textNotTo,textKeyWords;
    QString out_notToList, out_keyWords;

    out_notToList = ui->text_notToList->document()->toPlainText();
    out_keyWords = ui->text_keywords->document()->toPlainText();

    QString s_line(" ");
    QStringList s_list;
    QTextStream format(&out_notToList);

    while(!format.atEnd()) {
            s_line = format.readLine();
            s_list << s_line.trimmed();
    }

    s_list.sort();
    s_list.removeDuplicates();
    out_notToList.clear();

    for (int i = 0; i < s_list.size(); ++i) {
        if(s_list.at(i).isEmpty())
            continue;

        out_notToList.append(s_list.at(i));
        out_notToList.append("\n");
    }

    s_line = " ";
    QStringList s_list2;
    QTextStream format2(&out_keyWords);

    while(!format2.atEnd()) {
            s_line = format2.readLine();
            s_list2 << s_line.trimmed();
    }
    s_list2.sort();
    s_list2.removeDuplicates();
    out_keyWords.clear();

    for (int i = 0; i < s_list2.size(); ++i) {
        if(s_list2.at(i).isEmpty())
            continue;

         out_keyWords.push_back(s_list2.at(i));
         out_keyWords.push_back("\n");
    }

    msgBox.setText("Do you want update the lists?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    msgBox.setStandardButtons(QMessageBox::Ok);
    switch (ret) {
      case QMessageBox::Yes:
            if(!updateListOfWords(NOT_TO_LIST,out_notToList))
               return;
            if(!updateListOfWords(KEYWORDS_LIST,out_keyWords))
               return;

            msgBox.setText("Lists updated!");
            msgBox.exec();

            // update the view of "Not to" list
            textNotTo =  ui->text_notToList->textCursor();
            textNotTo.select(QTextCursor::Document);
            textNotTo.removeSelectedText();
            textNotTo.insertText(out_notToList);
            ui->text_notToList->setTextCursor(textNotTo);

            // update the view of keywords list
            textKeyWords =  ui->text_keywords->textCursor();
            textKeyWords.select(QTextCursor::Document);
            textKeyWords.removeSelectedText();
            textKeyWords.insertText(out_keyWords);
            ui->text_keywords->setTextCursor(textKeyWords);

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
        ui->text_adminPath->textCursor().document()->clear();
        ui->text_adminPath->textCursor().insertText(dir);
    }
}

void MainWindow::on_tree_chooseFile_doubleClicked(const QModelIndex &index)
{
   // QModelIndex indexPath;
    //indexPath = ui->tree_chooseFile->currentIndex();
    QString path;
    path = m_fileSystemModel->filePath(index);

    if(!path.isEmpty())
        m_fullPathNamesList->append(path);

    m_fullPathNamesList->removeDuplicates();
    m_fullPathNamesModel->setStringList((*m_fullPathNamesList));

    ui->tree_selectedFiles->setHeaderHidden(true);
    ui->tree_selectedFiles->setModel(m_fullPathNamesModel);
}

void MainWindow::on_pushButton_clicked()
{
    on_btn_unselectAll_clicked();


    m_fileSystemModel->setRootPath(QDir::currentPath());
    ui->tree_chooseFile->setModel(m_fileSystemModel);
    ui->tree_chooseFile->reset();

    ui->textEdit->textCursor().document()->clear();

    ui->stackedWidget->setCurrentIndex (0);

    ui->comboBox->setCurrentIndex (0);
}

void MainWindow::on_pushButton_2_clicked()
{
        ui->stackedWidget->setCurrentIndex (0);

}

void MainWindow::on_pushButton_3_clicked()
{
            string s_temp;
            QStringList s_list;
            g_engine->getFileManager()->loadFile(CONFIG_FILE,s_temp);

            QString s_input(s_temp.c_str());
            s_list = s_input.split(";");

            QString s_dir;
            s_dir = s_list.at(1);
            s_dir.append("/projectName");

            ui->comboBox->setCurrentIndex ( s_list.at(0).toInt() );
            userOptions->s_saveToDirectory = s_list.at(1).toStdString();
            ui->textEdit->textCursor().document()->clear();
            ui->textEdit->textCursor().insertText(s_dir);
            s_list.pop_front();
            s_list.pop_front();

            m_fullPathNamesList->clear();
            for (int i=0; i<s_list.size(); ++i) {
                m_fullPathNamesList->append( s_list.at(i) );
            }
            m_fullPathNamesModel->setStringList(*(m_fullPathNamesList));

            ui->tree_selectedFiles->setHeaderHidden(true);
            ui->tree_selectedFiles->setModel(m_fullPathNamesModel);

            if(userOptions->s_projectName.empty())
                userOptions->s_projectName = "ProjectName";
}
