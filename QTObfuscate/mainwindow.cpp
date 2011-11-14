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
    ui->comboBox->addItem(tr("--Algorithm--  (View directory)"));
    QStringList filters;
    QList<QFileInfo> fileList;
    filters << "*.so" << "*.dll";
    m_dir->setPath(DEFAULT_PLUGIN_PATH);
    //m_dir->setNameFilters(filters);
   fileList =  m_dir->entryInfoList(filters);

   if(fileList.empty())
       std::cout << "Cannot access file!" << std::endl;
   else
   {
       for (int i = 0; i < fileList.size(); ++i) {
           ui->comboBox->addItem(fileList[i].fileName());
        }
   }

   std::set<std::string> set_pluginDescription;
   g_engine->getFileManager()->loadFile(PLUGIN_INFO_PATH, set_pluginDescription);
   parseFile(set_pluginDescription, m_pluginDescription);

}

void MainWindow::parseFile(std::set<std::string>& set_input, std::map<std::string,std::string>& m_output)
{
        std::set<std::string>::iterator s_iter;
        for(s_iter=set_input.begin(); s_iter!=set_input.end(); ++s_iter)
        {
                std::size_t i_found=0,i_found2=0;
               std:: string s_value,s_value2;

                i_found = (*s_iter).find(":");
                i_found2 = (*s_iter).find(":",i_found+1);
                if(i_found != std::string::npos) {
                        s_value =  (*s_iter).substr(0, i_found);
                        s_value2 =  (*s_iter).substr(i_found+1, i_found2-i_found-1);

                        m_output.insert(std::pair<std::string,std::string>(s_value,s_value2));
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
   // g_engine->getPlugin()->setUpFile();
   // g_engine->getPlugin()->algorithm();
    std::string s_tempPluginFileName;
    s_tempPluginFileName = userOptions->s_pluginFileName;
    if(s_tempPluginFileName.find(".so")!=string::npos || s_tempPluginFileName.find(".dll")!=string::npos)
         g_engine->getPluginManager()->loadPlugin(s_tempPluginFileName);

//tree->setRootIndex(model->index(QDir::currentPath()));
    QModelIndex index;
    if(!userOptions->s_saveToDirectory.empty())
        index = m_fileSystemModel->index(userOptions->s_saveToDirectory.c_str());
    else
        index = m_fileSystemModel->setRootPath(QDir::currentPath());
    ui->treeView_4->setRootIndex(index);
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
    dir.append("/{$PROJECT_NAME}");
    ui->textEdit->textCursor().insertText(dir);
}

void MainWindow::on_pushButton_7_clicked()
{
    //g_engine->getPlugin2()->setUpFile();
    //g_engine->getPlugin2()->setUpMapping();

    //g_engine->getPlugin2()->algorithm();
    std::string s_tempPluginFileName;
    s_tempPluginFileName = userOptions->s_pluginFileName;
    g_engine->getPluginManager()->loadPlugin(s_tempPluginFileName);
}


void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    std::string s_pluginName;
    std::string s_pluginDescription = "";
    s_pluginName = arg1.toStdString();
    s_pluginDescription = m_pluginDescription[s_pluginName];
    //QTextDocument *textDocument = new QTextDocument;
    if(s_pluginDescription != "") {
        m_textDocument->setPlainText(s_pluginDescription.c_str());
        s_pluginDescription="";
        m_textEdit->setDocument(m_textDocument);
        //m_textEdit->setDocumentTitle(tr("Plugin Description"));
        ui->stackedWidget->insertWidget(ui->comboBox->currentIndex(),m_textEdit);
    }
    if(arg1.endsWith(".so") || arg1.endsWith(".dll"))
        userOptions->s_pluginFileName = DEFAULT_PLUGIN_PATH + s_pluginName;
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
