#include "selectcameraparamsdialog.h"
#include "ui_selectcameraparamsdialog.h"

/* Function SelectCameraParamsDialog
 * -------------------------------
 * build in constructor
 *
 * parent : parent widget.
*/
SelectCameraParamsDialog::SelectCameraParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCameraParamsDialog)
{
    ui->setupUi(this);
}

/* Function SelectCameraParamsDialog
 * -------------------------------
 * overloaded construtor that will set up the camera dialog
 *
 * parent : parent widget.
*/
SelectCameraParamsDialog::SelectCameraParamsDialog(int maxWidth,int maxHeight,int currentWidth,
                                                   int currentHeight,int xOffset,
                                                   int yOffset,int vBinning,int hBinning,
                                                   bool xFlip, bool yFlip)
{
    this->setDialog(maxWidth,maxHeight,currentWidth,currentHeight,
                    xOffset,yOffset,vBinning,hBinning,xFlip,yFlip);
}

/* Function SelectCameraParamsDialog
 * -------------------------------
 * overloaded construtor that will set up the camera dialog
 *
 * parent : parent widget.
*/
SelectCameraParamsDialog::SelectCameraParamsDialog(int maxWidth,int maxHeight,Rect cameraRect,
                                                   int vBinning,int hBinning,
                                                   bool xFlip, bool yFlip)
{
    this->setDialog(maxWidth,maxHeight,cameraRect.width,cameraRect.height,
                    cameraRect.x,cameraRect.y,vBinning,hBinning,xFlip,yFlip);
}

/* Function ~SelectCameraParamsDialog
 * -------------------------------
 * build in destructor
*/
SelectCameraParamsDialog::~SelectCameraParamsDialog()
{
    delete ui;
}

/* Function setDialog
 * -------------------------------
 * function that will set up the current parameters for the dialog
 *
 * parent : parent widget.
*/
void SelectCameraParamsDialog::setDialog(int maxWidth,int maxHeight,
                                         int currentWidth, int currentHeight,
                                         int xOffset,int yOffset,
                                         int vBinning,int hBinning,
                                         bool xFlip, bool yFlip)
{
    connect(this,SIGNAL(rejected()),SLOT(on_Cancel_pushButton_clicked()));
    m_maxWidth = maxWidth;
    m_maxHeight = maxHeight;
    m_currentWidth = currentWidth;
    m_currentHeight = currentHeight;
    m_xOffset = xOffset;
    m_yOffset = yOffset;
    m_vBinning = vBinning;
    m_hBinning = hBinning;
    m_xFlip = xFlip;
    m_yFlip = yFlip;

    ui->xFlip_checkBox->setChecked(m_xFlip);
    ui->yFlip_checkBox->setChecked(m_yFlip);

    ui->width_spinBox->setMaximum(m_maxWidth);
    ui->width_slider->setMaximum(m_maxWidth);
    ui->width_slider->setValue(currentWidth);

    ui->height_spinBox->setMaximum(m_maxHeight);
    ui->height_slider->setMaximum(m_maxHeight);
    ui->height_slider->setValue(currentHeight);

    ui->xOffset_slider->setValue(xOffset);
    ui->yOffset_slider->setValue(yOffset);

    ui->vBinning_slider->setValue(vBinning);
    ui->hBinning_slider->setValue(hBinning);
}

/* Function setDialog
 * -------------------------------
 * function that will set up the current parameters for the dialog
 *
 * parent : parent widget.
*/
void SelectCameraParamsDialog::setDialog(int maxWidth,int maxHeight,Rect cameraRect,
                                         int vBinning,int hBinning,
                                         bool xFlip, bool yFlip)
{
    this->setDialog(maxWidth,maxHeight,cameraRect.width,cameraRect.height,
                    cameraRect.x,cameraRect.y,vBinning,hBinning,xFlip,yFlip);
}

/* Private slot on_Load_pushButton_clicked
 * -------------------------------
 * slot callen when the load button is clicked
 * it calls to the inherited function accept that
 * will emit a accepted signal
*/
void SelectCameraParamsDialog::on_Load_pushButton_clicked()
{
    this->accept();
}

/* Private slot on_Cancel_pushButton_clicked
 * -------------------------------
 * slot called the cancel button is clicked
 * it calls to the inherited function reject that
 * will emit a rejected signal
*/
void SelectCameraParamsDialog::on_Cancel_pushButton_clicked()
{
    this->close();
    this->disconnect();
}

//Height and width stuff


void SelectCameraParamsDialog::on_width_slider_valueChanged(int position)
{
    //the camera doesn't allow odd size
    m_currentWidth = position;

    ui->xOffset_slider->setMaximum(m_maxWidth/m_hBinning-m_currentWidth);
    ui->xOffset_spinBox->setMaximum(m_maxWidth/m_hBinning-m_currentWidth);
    m_xOffset = ui->xOffset_spinBox->value();

    ui->width_slider->setValue(m_currentWidth);
    ui->width_spinBox->setValue(m_currentWidth);
}
void SelectCameraParamsDialog::on_width_spinBox_valueChanged(int arg1)
{
    m_currentWidth = arg1;

    ui->width_spinBox->setValue(m_currentWidth);
    ui->width_slider->setValue(m_currentWidth);

    ui->xOffset_slider->setMaximum(m_maxWidth/m_hBinning-m_currentWidth);
    ui->xOffset_spinBox->setMaximum(m_maxWidth/m_hBinning-m_currentWidth);
    m_xOffset = ui->xOffset_spinBox->value();
}

void SelectCameraParamsDialog::on_height_slider_valueChanged(int position)
{
    m_currentHeight = position;

    ui->yOffset_slider->setMaximum(m_maxHeight/m_vBinning-m_currentHeight);
    ui->yOffset_spinBox->setMaximum(m_maxHeight/m_vBinning-m_currentHeight);
    m_yOffset = ui->yOffset_spinBox->value();

    ui->height_slider->setValue(m_currentHeight);
    ui->height_spinBox->setValue(m_currentHeight);
}
void SelectCameraParamsDialog::on_height_spinBox_valueChanged(int arg1)
{
    m_currentHeight = arg1;

    ui->height_slider->setValue(m_currentHeight);
    ui->height_spinBox->setValue(m_currentHeight);

    ui->yOffset_slider->setMaximum(m_maxHeight/m_vBinning-m_currentHeight);
    ui->yOffset_spinBox->setMaximum(m_maxHeight/m_vBinning-m_currentHeight);
    m_yOffset = ui->yOffset_spinBox->value();
}

//Vertical and horizontal binning
void SelectCameraParamsDialog::on_vBinning_slider_valueChanged(int position)
{
    m_vBinning = position;
    ui->vBinning_spinBox->setValue(position);

    ui->height_slider->setMaximum(m_maxHeight/position);
    ui->height_spinBox->setMaximum(m_maxHeight/position);
    ui->yOffset_slider->setMaximum(m_maxHeight/position-m_currentHeight);
    ui->yOffset_spinBox->setMaximum(m_maxHeight/position-m_currentHeight);
}
void SelectCameraParamsDialog::on_vBinning_spinBox_valueChanged(int arg1)
{
    m_vBinning = arg1;
    ui->vBinning_slider->setValue(arg1);

    ui->height_slider->setMaximum(m_maxHeight/arg1);
    ui->height_spinBox->setMaximum(m_maxHeight/arg1);
    ui->yOffset_slider->setMaximum(m_maxHeight/arg1-m_currentHeight);
    ui->yOffset_spinBox->setMaximum(m_maxHeight/arg1-m_currentHeight);
}

void SelectCameraParamsDialog::on_hBinning_slider_valueChanged(int position)
{
    m_hBinning = position;
    ui->hBinning_spinBox->setValue(position);

    ui->width_slider->setMaximum(m_maxWidth/position);
    ui->width_spinBox->setMaximum(m_maxWidth/position);
    ui->xOffset_slider->setMaximum(m_maxWidth/position-m_currentWidth);
    ui->xOffset_spinBox->setMaximum(m_maxWidth/position-m_currentWidth);
}
void SelectCameraParamsDialog::on_hBinning_spinBox_valueChanged(int arg1)
{
    m_hBinning = arg1;
    ui->hBinning_slider->setValue(arg1);

    ui->width_slider->setMaximum(m_maxWidth/arg1);
    ui->width_spinBox->setMaximum(m_maxWidth/arg1);
    ui->xOffset_slider->setMaximum(m_maxWidth/arg1-m_currentWidth);
    ui->xOffset_spinBox->setMaximum(m_maxWidth/arg1-m_currentWidth);
}

//Offset stuff
void SelectCameraParamsDialog::on_xOffset_slider_valueChanged(int position)
{
    m_xOffset = position;

    ui->xOffset_slider->setValue(m_xOffset);
    ui->xOffset_spinBox->setValue(m_xOffset);
}

void SelectCameraParamsDialog::on_xOffset_spinBox_valueChanged(int arg1)
{
    m_xOffset = arg1;

    ui->xOffset_spinBox->setValue(m_xOffset);
    ui->xOffset_slider->setValue(m_xOffset);
}

void SelectCameraParamsDialog::on_yOffset_slider_valueChanged(int position)
{
    m_yOffset = position;

    ui->yOffset_slider->setValue(m_yOffset);
    ui->yOffset_spinBox->setValue(m_yOffset);
}
void SelectCameraParamsDialog::on_yOffset_spinBox_valueChanged(int arg1)
{
    m_yOffset = arg1;

    ui->yOffset_slider->setValue(m_yOffset);
    ui->yOffset_slider->setValue(m_yOffset);
}

void SelectCameraParamsDialog::on_fromYAML_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Open Camera Parameters"),
                "",
                tr("YAML Files (*.yml *.yaml);;All Files (*)" )
                );
    if(path.length()>0)
    {
        int maxHeight, maxWidth = 0;
        FileStorage fs(path.toStdString(), FileStorage::READ);
        fs["horizontalBinning"] >> m_hBinning;
        fs["verticalBinning"] >> m_vBinning;
        fs["xflip"] >> m_xFlip;
        fs["yflip"] >> m_yFlip;
        fs["width"] >> m_currentWidth;
        fs["height"] >> m_currentHeight;
        fs["maxWidth"] >> maxWidth;
        fs["maxHeight"] >> maxHeight;
        fs["xOffset"] >> m_xOffset;
        fs["yOffset"] >> m_yOffset;

        if(maxWidth != m_maxWidth || maxHeight != m_maxHeight)
        {
            QMessageBox Msgbox;
            Msgbox.setIcon(Msgbox.Warning);
            Msgbox.setText("<big>Warning</big> <p>\n\n Camera Resolution aren't equal \n It's posible that the YALM file is from other camera</p>");
            Msgbox.exec();
        }

        on_hBinning_slider_valueChanged(m_hBinning);
        on_vBinning_slider_valueChanged(m_vBinning);

        on_width_slider_valueChanged(m_currentWidth);
        on_height_slider_valueChanged(m_currentHeight);

        on_xOffset_slider_valueChanged(m_xOffset);
        on_yOffset_slider_valueChanged(m_yOffset);

        ui->xFlip_checkBox->setChecked(m_xFlip);
        ui->yFlip_checkBox->setChecked(m_yFlip);
    }
}

void SelectCameraParamsDialog::on_xFlip_checkBox_clicked()
{
    this->m_xFlip = ui->xFlip_checkBox->isChecked();
}

void SelectCameraParamsDialog::on_yFlip_checkBox_clicked()
{
    this->m_yFlip = ui->yFlip_checkBox->isChecked();
}


Rect SelectCameraParamsDialog::getcurrentRect()
{
    return Rect(m_xOffset,m_yOffset,m_currentWidth,m_currentHeight);
}

int SelectCameraParamsDialog::getMaxWidth()
{
    return m_maxWidth;
}

int SelectCameraParamsDialog::getMaxHeight()
{
    return m_maxHeight;
}

int SelectCameraParamsDialog::getcurrentWidth()
{
    return m_currentWidth;
}

int SelectCameraParamsDialog::getcurrentHeight()
{
    return m_currentHeight;
}

int SelectCameraParamsDialog::getXOffset()
{
    return m_xOffset;
}

int SelectCameraParamsDialog::getYOffset()
{
    return m_yOffset;
}
int SelectCameraParamsDialog::getHBinning()
{
    return m_hBinning;
}

int SelectCameraParamsDialog::getVBinning()
{
    return m_vBinning;
}

bool SelectCameraParamsDialog::getXFlip()
{
    return m_xFlip;
}
bool SelectCameraParamsDialog::getYFlip()
{
    return m_yFlip;
}

int SelectCameraParamsDialog::odd2Even(int num)
{
    if(num%2 != 0)
        num = num - 1;
    else
        num = num;
    return num;
}

void SelectCameraParamsDialog::on_save_YAML_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save YAML"),
                               "",
                               tr("YAML File (*.yml *.yaml)"));

    FileStorage fs(fileName.toStdString(), FileStorage::WRITE);

    fs << "horizontalBinning" << m_hBinning;
    fs << "verticalBinning" << m_vBinning;
    fs << "xflip" << m_xFlip;
    fs << "yflip" << m_yFlip;
    fs << "width" << m_currentWidth;
    fs << "height" << m_currentHeight;
    fs << "maxWidth" << m_maxWidth;
    fs << "maxHeight" << m_maxHeight;
    fs << "xOffset" << m_xOffset;
    fs << "yOffset" << m_yOffset;
}
