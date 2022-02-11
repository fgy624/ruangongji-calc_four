/********************************************************************************
** Form generated from reading UI file 'calc_card.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALC_CARD_H
#define UI_CALC_CARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Calc_card
{
public:
    QLabel *label;
    QLineEdit *answer_line;
    QPushButton *submit;
    QLabel *score_lable;
    QLabel *score;
    QLCDNumber *clock;
    QLabel *question;
    QLabel *label_2;

    void setupUi(QWidget *Calc_card)
    {
        if (Calc_card->objectName().isEmpty())
            Calc_card->setObjectName(QStringLiteral("Calc_card"));
        Calc_card->resize(728, 598);
        Calc_card->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 170, 127);"));
        label = new QLabel(Calc_card);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(200, 400, 131, 31));
        label->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        answer_line = new QLineEdit(Calc_card);
        answer_line->setObjectName(QStringLiteral("answer_line"));
        answer_line->setGeometry(QRect(340, 400, 151, 31));
        submit = new QPushButton(Calc_card);
        submit->setObjectName(QStringLiteral("submit"));
        submit->setGeometry(QRect(260, 480, 161, 51));
        submit->setStyleSheet(QLatin1String("color: rgb(136, 136, 136);\n"
"color: rgb(0, 0, 0);\n"
"background-color: rgb(94, 94, 94);"));
        score_lable = new QLabel(Calc_card);
        score_lable->setObjectName(QStringLiteral("score_lable"));
        score_lable->setGeometry(QRect(450, 50, 121, 41));
        score_lable->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        score = new QLabel(Calc_card);
        score->setObjectName(QStringLiteral("score"));
        score->setGeometry(QRect(607, 50, 81, 41));
        clock = new QLCDNumber(Calc_card);
        clock->setObjectName(QStringLiteral("clock"));
        clock->setGeometry(QRect(320, 50, 101, 51));
        question = new QLabel(Calc_card);
        question->setObjectName(QStringLiteral("question"));
        question->setGeometry(QRect(210, 220, 311, 61));
        label_2 = new QLabel(Calc_card);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(290, 150, 111, 31));

        retranslateUi(Calc_card);

        QMetaObject::connectSlotsByName(Calc_card);
    } // setupUi

    void retranslateUi(QWidget *Calc_card)
    {
        Calc_card->setWindowTitle(QApplication::translate("Calc_card", "calc_card", Q_NULLPTR));
        label->setText(QApplication::translate("Calc_card", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\347\255\224\346\241\210", Q_NULLPTR));
        submit->setText(QApplication::translate("Calc_card", "\346\217\220\344\272\244\347\255\224\346\241\210", Q_NULLPTR));
        score_lable->setText(QApplication::translate("Calc_card", "\345\276\227\345\210\206", Q_NULLPTR));
        score->setText(QString());
        question->setText(QString());
        label_2->setText(QApplication::translate("Calc_card", "\351\242\230\347\233\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Calc_card: public Ui_Calc_card {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALC_CARD_H
