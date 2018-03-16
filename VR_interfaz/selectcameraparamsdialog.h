#ifndef SELECTCAMERAPARAMSDIALOG_H
#define SELECTCAMERAPARAMSDIALOG_H

#include <QDialog>
#include "opencv2/core/core.hpp"
#include "QFileDialog"
#include <QDebug>
#include "QMessageBox"

using namespace cv;

namespace Ui {
class SelectCameraParamsDialog;
}

/* Class SelectCameraParamsDialog
 * -------------------------------
 * this class is used to create a dialog to allow the user
 * to easily change the camera parameters
 * uses the Q_OBJECT macro
*/
class SelectCameraParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCameraParamsDialog(QWidget *parent = 0);
    SelectCameraParamsDialog(int maxWidth,int maxHeight,
                             int currentWidth, int currentHeight,
                             int xOffset=0,int yOffset=0,
                             int vBinning=1,int hBinning=1,
                             bool xFlip = false, bool yFlip = false);
    SelectCameraParamsDialog(int maxWidth,int maxHeight,Rect cameraRect,
                             int vBinning=1,int hBinning=1,
                             bool xFlip = false, bool yFlip = false);
    ~SelectCameraParamsDialog();

    void setDialog(int maxWidth,int maxHeight,
                   int currentWidth, int currentHeight,
                   int xOffset=0,int yOffset=0,
                   int vBinning=1,int hBinning=1,
                   bool xFlip = false, bool yFlip = false);
    void setDialog(int maxWidth,int maxHeight,Rect cameraRect,
                   int vBinning=1,int hBinning=1,
                   bool xFlip = false, bool yFlip = false);

    Rect getcurrentRect();
    int getMaxWidth();
    int getMaxHeight();
    int getcurrentWidth();
    int getcurrentHeight();
    int getXOffset();
    int getYOffset();
    int getHBinning();
    int getVBinning();
    bool getXFlip();
    bool getYFlip();

private slots:
    void on_Load_pushButton_clicked();
    void on_Cancel_pushButton_clicked();

    void on_hBinning_slider_valueChanged(int position);
    void on_hBinning_spinBox_valueChanged(int arg1);
    void on_vBinning_slider_valueChanged(int position);
    void on_vBinning_spinBox_valueChanged(int arg1);

    void on_xOffset_slider_valueChanged(int position);
    void on_xOffset_spinBox_valueChanged(int arg1);
    void on_yOffset_slider_valueChanged(int position);
    void on_yOffset_spinBox_valueChanged(int arg1);

    void on_width_slider_valueChanged(int position);
    void on_width_spinBox_valueChanged(int arg1);
    void on_height_slider_valueChanged(int position);
    void on_height_spinBox_valueChanged(int arg1);

    void on_fromYAML_pushButton_clicked();
    void on_save_YAML_pushButton_clicked();

    void on_xFlip_checkBox_clicked();
    void on_yFlip_checkBox_clicked();



private:
    Ui::SelectCameraParamsDialog *ui;
    int odd2Even(int num);

    int m_maxWidth;
    int m_maxHeight;
    int m_currentWidth;
    int m_currentHeight;
    int m_xOffset;
    int m_yOffset;
    int m_vBinning;
    int m_hBinning;

    bool m_xFlip;
    bool m_yFlip;
};

#endif // SELECTCAMERAPARAMSDIALOG_H
