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
    m_leftPath = ui->lineEdit_left->text();
    m_rightPath = ui->lineEdit_right->text();

    m_leftPath.replace("\\","/");
    m_rightPath.replace("\\","/");

    qDebug() << m_leftPath.toLatin1().data();
    qDebug() << m_rightPath.toLatin1().data();
    qDebug() << ui->spinBox_col->value() << ui->spinBox_row->value();
    qDebug() << ui->spinBox_numImg->value();
    qDebug() << ui->spinBox_squareSize->value();

    char namefile[] = "";
    char extension[] = "png";

    m_leftCam.calibrateFromImages(ui->spinBox_row->value(),ui->spinBox_col->value(),ui->spinBox_numImg->value(),
                                  ui->spinBox_squareSize->value(),m_leftPath.toLatin1().data(),namefile,extension);

    m_rightCam.calibrateFromImages(ui->spinBox_col->value(),ui->spinBox_row->value(),ui->spinBox_numImg->value(),
                                   ui->spinBox_squareSize->value(), m_rightPath.toLatin1().data(),namefile,extension);

    m_stereoCalib.calibrateStereoFromImage(m_leftCam,m_rightCam,ui->spinBox_col->value(),ui->spinBox_row->value(),
                                           ui->spinBox_numImg->value(),ui->spinBox_squareSize->value(),
                                           m_leftPath.toLatin1().data(),m_rightPath.toLatin1().data(),namefile,
                                           namefile,extension);

    if(!m_leftCam.isCalibrated() || !m_rightCam.isCalibrated() || !m_stereoCalib.isCalibrated())
    {
        QMessageBox Msgbox;
        Msgbox.setIcon(Msgbox.Warning);
        Msgbox.setText("<big>Warning</big> <p>\n\n Cameras not calibrated sucessfully</p>");
        Msgbox.exec();
        this->reject();
    } else {
        //this has to be moved to the calib dialog ui to allow customization in de namefiles
        char leftOut[] = "calibLeft.yml";
        char rightOut[] = "calibRight.yml";
        char stereoOut[] = "calibStereo.yml";

        m_leftCam.saveParamsInFile(leftOut);
        m_rightCam.saveParamsInFile(rightOut);
        m_stereoCalib.saveParamsInFile(stereoOut);

        this->accept();
    }
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

CameraCalibration CalibDialog::getLeftCalibration()
{
    return m_leftCam;
}
CameraCalibration CalibDialog::getRightCalibration()
{
    return m_rightCam;
}

StereoCalibration CalibDialog::getStereoCalibration()
{
    return m_stereoCalib;
}









