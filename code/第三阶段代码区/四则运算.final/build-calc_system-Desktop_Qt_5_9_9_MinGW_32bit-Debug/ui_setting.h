/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Setting
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLabel *label_3;
    QPushButton *confirm;

    void setupUi(QWidget *Setting)
    {
        if (Setting->objectName().isEmpty())
            Setting->setObjectName(QStringLiteral("Setting"));
        Setting->resize(696, 501);
        Setting->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label = new QLabel(Setting);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 100, 161, 61));
        label->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        lineEdit = new QLineEdit(Setting);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(340, 120, 151, 31));
        label_2 = new QLabel(Setting);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(160, 250, 131, 31));
        label_2->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        radioButton = new QRadioButton(Setting);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(370, 230, 112, 21));
        radioButton->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        radioButton_2 = new QRadioButton(Setting);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(370, 300, 112, 21));
        radioButton_2->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_3 = new QLabel(Setting);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(520, 120, 141, 31));
        label_3->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        confirm = new QPushButton(Setting);
        confirm->setObjectName(QStringLiteral("confirm"));
        confirm->setGeometry(QRect(240, 370, 211, 71));
        QFont font;
        font.setPointSize(20);
        confirm->setFont(font);
        confirm->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 255, 0);"));

        retranslateUi(Setting);

        QMetaObject::connectSlotsByName(Setting);
    } // setupUi

    void retranslateUi(QWidget *Setting)
    {
        Setting->setWindowTitle(QApplication::translate("Setting", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("Setting", "\351\242\230\347\233\256\346\225\260\351\207\217\350\256\276\347\275\256", Q_NULLPTR));
        label_2->setText(QApplication::translate("Setting", "\344\271\230\346\226\271\350\256\276\347\275\256", Q_NULLPTR));
        radioButton->setText(QApplication::translate("Setting", "**", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("Setting", "^", Q_NULLPTR));
        label_3->setText(QApplication::translate("Setting", "\346\234\200\345\244\2321000\351\201\223", Q_NULLPTR));
        confirm->setText(QApplication::translate("Setting", "\347\241\256\345\256\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Setting: public Ui_Setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
