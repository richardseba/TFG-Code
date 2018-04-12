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
    m_leftPath = dir;
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
    m_rightPath = dir;
}

/* Private slot on_pushButton_calibrate_clicked
 * -------------------------------
 * slot called when the calibration button is clicked
 * it calls a python script that will create the calibration parameters files
*/
void CalibDialog::on_pushButton_calibrate_clicked()
{
    m_leftPath.replace("\\","/");
    m_rightPath.replace("\\","/");



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
