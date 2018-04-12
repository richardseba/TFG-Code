 #include "loadcalibparamsdialog.h"
#include "ui_loadcalibparamsdialog.h"

/* Function LoadCalibParamsDialog
 * -------------------------------
 * built in constructor
 * sets up the ui for the dialog
*/
LoadCalibParamsDialog::LoadCalibParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadCalibParamsDialog)
{
    ui->setupUi(this);
}

/* Function ~CalibDialog
 * -------------------------------
 * built in destructor
 * destroys the ui
*/
LoadCalibParamsDialog::~LoadCalibParamsDialog()
{
    delete ui;
}

/* Private slot on_pushButton_load_clicked
 * -------------------------------
 * slot called the Load button is clicked
 * it calls to the inherited function reject that
 * will emit a accepted signal
*/
void LoadCalibParamsDialog::on_pushButton_load_clicked()
{
    this->accept();
}

/* Private slot on_pushButton_cancel_clicked
 * -------------------------------
 * slot called the cancel button is clicked
 * it calls to the inherited function reject that
 * will emit a rejected signal
*/
void LoadCalibParamsDialog::on_pushButton_cancel_clicked()
{
    this->reject();
}

/* Public Function getCamLConfigPath
 * -------------------------------
 * returns the current calibration matrix file path for the left camera
*/
QString LoadCalibParamsDialog::getCamLConfigPath()
{
    return ui->lineEdit_matrixL->text();
}

/* Public Function getCamRConfigPath
 * -------------------------------
 * returns the current calibration matrix file path for the right camera
*/
QString LoadCalibParamsDialog::getCamRConfigPath()
{
    return ui->lineEdit_matrixR->text();
}


QString LoadCalibParamsDialog::getStereoConfigPath()
{
    return ui->lineEdit_stereoCalib->text();
}

/* Private slot on_pushButton_matrixR_clicked
 * -------------------------------
 * slot called when the calibration right camera path button is clicked
 * it opens a file explorer dialog to allow the user to search
 * for the desired file path
*/
void LoadCalibParamsDialog::on_pushButton_matrixR_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Calibration file of Camera Right"),
                "",
                tr("YAML Files (*.yml);;Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_matrixR->setText(path);
}

/* Private slot on_pushButton_matrixL_clicked
 * -------------------------------
 * slot called when the intrinsics right camera path button is clicked
 * it opens a file explorer dialog to allow the user to search
 * for the desired file path
*/
void LoadCalibParamsDialog::on_pushButton_matrixL_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Calibration file of Camera Left"),
                "",
                tr("YAML Files (*.yml);;Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_matrixL->setText(path);
}

void LoadCalibParamsDialog::on_pushButton_stereo_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Calibration file for the stereo parameters"),
                "",
                tr("YAML Files (*.yml);;Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_stereoCalib->setText(path);
}
