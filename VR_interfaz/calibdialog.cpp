#include "calibdialog.h"
#include "ui_calibdialog.h"
#include "QDebug"

/* Function CalibDialog
 * -------------------------------
 * built in constructor
 * sets up the ui for the dialog
*/
CalibDialog::CalibDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibDialog)
{
    ui->setupUi(this);
}

/* Function ~CalibDialog
 * -------------------------------
 * built in destructor
 * destroys the ui
*/
CalibDialog::~CalibDialog()
{
    delete ui;
}

/* Private slot on_pushButton_leftpath_clicked
 * -------------------------------
 * slot called when the left camera path button is clicked
 * it opens a file explorer dialog to allow the user to search
 * for the desired folder path
*/
void CalibDialog::on_pushButton_leftpath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Camera Left"),
                                                 "./",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_left->setText(dir);
}

/* Private slot on_pushButton_rightpath_clicked
 * -------------------------------
 * slot called when the right camera path button is clicked
 * it opens a file explorer dialog to allow the user to search
 * for the desired folder path
*/
void CalibDialog::on_pushButton_rightpath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Camera Right"),
                                                 "./",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_right->setText(dir);
}

/* Private slot on_pushButton_calibrate_clicked
 * -------------------------------
 * slot called when the calibration button is clicked
 * it calls a python script that will create the calibration parameters files
*/
void CalibDialog::on_pushButton_calibrate_clicked()
{
    QProcess process;
    QStringList params;
    params << ui->lineEdit_script->text();
    params << QVariant(ui->spinBox_col->value()).toString();
    params << QVariant(ui->spinBox_row->value()).toString();
    params << ui->lineEdit_left->text().replace("\\","/");
    params << ui->lineEdit_right->text().replace("\\","/");

//    process.start(ui->lineEdit_python->text(),params);
//    process.waitForFinished();
//    qDebug() << process.readAllStandardOutput();
    process.startDetached(ui->lineEdit_python->text(),params);
    this->accept();
}

/* Private slot on_pushButton_Cancel_clicked
 * -------------------------------
 * slot called when the cancel button is clicked
 * it calls to the inherited function reject that
 * will emit a rejected signal
*/
void CalibDialog::on_pushButton_Cancel_clicked()
{
    this->reject();
}

/* Private slot on_pushButton_pythonexe_clicked
 * -------------------------------
 * slot called when the python exe button is clicked
 * it opens a file explorer dialog.
*/
void CalibDialog::on_pushButton_pythonexe_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Python Executable"),
                "",
                tr("Executable (*.exe);;All Files (*)" )
                );
    ui->lineEdit_python->setText(path);
}

/* Private slot on_pushButton_pythonexe_clicked
 * -------------------------------
 * slot called when the script button is clicked
 * it opens a file explorer dialog to let the user set the file path
 * for the script that will calculate the calibration parameters
*/
void CalibDialog::on_pushButton_script_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Python Calibration Script"),
                "",
                tr("Python Files (*.py);;Python compiled Files (*.pyc);;All Files (*)" )
                );
    ui->lineEdit_script->setText(path);
}
