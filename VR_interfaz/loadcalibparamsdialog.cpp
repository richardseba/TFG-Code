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

/* Public Function getCamL_matrix_path
 * -------------------------------
 * returns the current calibration matrix file path for the left camera
*/
QString LoadCalibParamsDialog::getCamL_matrix_path()
{
    return ui->lineEdit_matrixL->text();
}

/* Public Function getCamL_distorsion_path
 * -------------------------------
 * returns the current distortion vector file path for the left camera
*/
QString LoadCalibParamsDialog::getCamL_distorsion_path()
{
    return ui->lineEdit_distL->text();
}

/* Public Function getCamL_matrix_path
 * -------------------------------
 * returns the current calibration matrix file path for the right camera
*/
QString LoadCalibParamsDialog::getCamR_matrix_path()
{
    return ui->lineEdit_matrixR->text();
}

/* Public Function getCamL_distorsion_path
 * -------------------------------
 * returns the current distortion vector file path for the right camera
*/
QString LoadCalibParamsDialog::getCamR_distorsion_path()
{
    return ui->lineEdit_distR->text();
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
                tr("Open Calibration intrinsics of Camera Right"),
                "",
                tr("Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_matrixR->setText(path);
}

/* Private slot on_pushButton_distR_clicked
 * -------------------------------
 * slot called when the distortion right camera path button is clicked
 * it opens a file explorer dialog to allow the user to search
 * for the desired file path
*/
void LoadCalibParamsDialog::on_pushButton_distR_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Calibration distorsion of Camera Right"),
                "",
                tr("Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_distR->setText(path);
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
                tr("Open Calibration intrinsics of Camera Left"),
                "",
                tr("Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_matrixL->setText(path);
}

/* Private slot on_pushButton_distL_clicked
 * -------------------------------
 * slot called when the distortion left camera path button is clicked
 * it opens a file explorer dialog to allow the user to search
 * for the desired file path
*/
void LoadCalibParamsDialog::on_pushButton_distL_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Calibration distorsion of Camera Left"),
                "",
                tr("Text Files (*.txt);;All Files (*)" )
                );
    ui->lineEdit_distL->setText(path);
}
