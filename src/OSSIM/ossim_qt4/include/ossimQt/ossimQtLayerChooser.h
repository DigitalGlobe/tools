#ifndef OSSIMQTLAYERCHOOSER_H
#define OSSIMQTLAYERCHOOSER_H

#include <QtCore/QVariant>


#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "ossim/base/ossimConnectableObject.h"
#include "ossimQtDataManagerListBox.h"
#include <vector>

class Ui_ossimQtLayerChooser
{
public:
    QVBoxLayout *vboxLayout;
    ossimQtDataManagerListBox *theDataManagerListBox;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *theApplyButton;
    QPushButton *theCancelButton;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *ossimQtLayerChooser)
    {
    ossimQtLayerChooser->setObjectName(QString::fromUtf8("ossimQtLayerChooser"));
    ossimQtLayerChooser->resize(QSize(620, 242).expandedTo(ossimQtLayerChooser->minimumSizeHint()));
    vboxLayout = new QVBoxLayout(ossimQtLayerChooser);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theDataManagerListBox = new ossimQtDataManagerListBox(ossimQtLayerChooser);
    theDataManagerListBox->setObjectName(QString::fromUtf8("theDataManagerListBox"));

    vboxLayout->addWidget(theDataManagerListBox);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    theApplyButton = new QPushButton(ossimQtLayerChooser);
    theApplyButton->setObjectName(QString::fromUtf8("theApplyButton"));

    hboxLayout->addWidget(theApplyButton);

    theCancelButton = new QPushButton(ossimQtLayerChooser);
    theCancelButton->setObjectName(QString::fromUtf8("theCancelButton"));

    hboxLayout->addWidget(theCancelButton);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout);

    retranslateUi(ossimQtLayerChooser);

    QMetaObject::connectSlotsByName(ossimQtLayerChooser);
    } // setupUi

    void retranslateUi(QDialog *ossimQtLayerChooser)
    {
    ossimQtLayerChooser->setWindowTitle(QApplication::translate("ossimQtLayerChooser", "Layer Chooser", 0, QApplication::UnicodeUTF8));
    theApplyButton->setText(QApplication::translate("ossimQtLayerChooser", "Apply", 0, QApplication::UnicodeUTF8));
    theCancelButton->setText(QApplication::translate("ossimQtLayerChooser", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtLayerChooser);
    } // retranslateUi


protected:
    enum IconID
    {
        image0_ID,
        unknown_ID
    };
    static QPixmap icon(IconID id)
    {
    static const char* const image0_data[] = { 
"22 22 208 2",
"Qt c None",
".u c #000000",
".Q c #020202",
"#g c #020204",
"#. c #020206",
".9 c #020208",
".Z c #02020a",
".P c #02020c",
".R c #02020d",
"ay c #040202",
"a. c #040206",
"#w c #04020c",
"az c #040404",
"ar c #04040c",
"#f c #04040d",
".e c #060606",
"aK c #08060c",
"ak c #0a0808",
".0 c #0d0f10",
"#x c #101012",
"aj c #101213",
"#5 c #121212",
"aT c #13150c",
"al c #13150d",
"#N c #151612",
"ax c #151812",
"aV c #151d0f",
"bn c #161904",
"#s c #161912",
"#3 c #161c13",
".3 c #181c13",
"#6 c #1a1a16",
"aS c #1a1c10",
"#W c #1c1d16",
"#7 c #1c2210",
"ai c #1c2212",
"aU c #1e2513",
"ac c #212616",
"aL c #222715",
".S c #222815",
"#E c #282d13",
"aG c #283015",
"#j c #2a2f13",
"ba c #2a3107",
"bm c #2a3108",
".O c #2b3115",
"#M c #2b3315",
"#B c #2c321a",
"#I c #2f331a",
".1 c #2f3418",
"#T c #313918",
".2 c #313a19",
".t c #374109",
".d c #383838",
"#9 c #383f19",
"aJ c #383f1a",
"#C c #404c18",
"af c #424e19",
"#k c #454e1c",
"## c #454f1d",
".J c #4c590c",
".4 c #4f5b1d",
"aF c #50591d",
"aH c #505b1d",
"#P c #505c1d",
"#4 c #516120",
"bc c #525f0f",
".7 c #52600d",
"aI c #53611e",
"bd c #53620d",
"bb c #53620e",
"#Z c #536221",
"#p c #53631e",
".W c #54620e",
"aX c #55630e",
"bl c #556310",
"be c #56640f",
"aA c #566520",
"#u c #57650f",
"bf c #59680f",
".Y c #59681c",
"#G c #5a690f",
"ad c #5a6922",
"bg c #5b6a10",
"#e c #5b6a22",
"#R c #5b6b11",
"#0 c #5c6a22",
"bh c #5d6c13",
"aC c #5d6d0e",
"bj c #5d6d10",
"#1 c #5d6d13",
"a5 c #5e6a22",
"#Q c #5e6b1e",
"an c #5e6d0f",
"bi c #5e6d11",
".c c #5f5f5f",
"a4 c #5f6a20",
"aa c #5f6e11",
"bk c #617110",
"aN c #617210",
"#r c #647322",
"as c #647423",
"#H c #64761a",
"#S c #657420",
".C c #657520",
".B c #65761a",
"aR c #667227",
"#h c #667326",
".T c #677922",
"aq c #6a7622",
".b c #6d6d6d",
".8 c #6e7d20",
".N c #6e7d21",
"a3 c #768822",
"#2 c #768921",
"a6 c #778728",
"#F c #778823",
"a# c #778828",
"#v c #778921",
".D c #778925",
".A c #778a21",
"ag c #788925",
".a c #7a7a7a",
".L c #88a213",
"#a c #899c20",
".X c #89a016",
".x c #89a018",
"#V c #8ca326",
".w c #8da321",
".z c #8da41e",
"#o c #8da426",
"aw c #8fa332",
"#8 c #8fa431",
"ah c #90a42c",
"#X c #90a62a",
".y c #90a916",
".M c #90a91c",
"ab c #91a91d",
".E c #91aa1e",
"#A c #91aa22",
"#L c #92a926",
"aW c #92ab29",
"b. c #92ab2a",
".# c #939393",
".s c #94a53e",
"b# c #96af1d",
"#d c #96b028",
"aD c #97b01a",
".G c #97b119",
"aY c #97b11a",
".H c #97b21a",
"a9 c #97b22a",
".I c #98b11a",
"aM c #98b22b",
"#m c #99b415",
"#y c #99b512",
"#J c #9ab121",
"#b c #9ab221",
"a2 c #9bb028",
"#l c #9bb322",
"aZ c #9bb51c",
"ap c #9cb51a",
".F c #9cb61a",
".V c #9cb71a",
"#t c #9db12a",
"aP c #9db519",
"#z c #9db815",
"#K c #9db915",
"#U c #9db91e",
".U c #9eb825",
"#n c #9eba13",
"#c c #9eba1e",
".v c #9fb046",
"ae c #9fb92b",
"a0 c #9fba1d",
"a7 c #a0b827",
"#q c #a0ba2c",
".6 c #a0bb19",
"aE c #a1b425",
"am c #a1b927",
"aQ c #a1ba21",
".5 c #a1bb20",
"at c #a3bd2c",
"#D c #a3bd2d",
"a1 c #a9c223",
"#i c #aac428",
"au c #adcb22",
"#O c #aecc25",
".i c #afbe63",
".j c #b0bf64",
".k c #b1c164",
"av c #b2d41d",
"#Y c #b2d420",
".h c #b4c170",
"aO c #b4c55d",
".r c #b4c65d",
".K c #b5c170",
".l c #b5c564",
".g c #b7c18b",
"ao c #b7c664",
".p c #b7c764",
".m c #b7c765",
".q c #b7c865",
".f c #b8bca7",
".o c #b8c865",
".n c #b8c965",
"a8 c #bdde25",
"aB c #bede23",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQt.#.a.b.b.b.b.b.b.b.b.b.b.c.d.eQtQtQt",
"QtQtQt.f.g.h.i.j.k.l.m.n.o.p.p.q.r.s.t.uQtQt",
"QtQt.#.g.v.w.x.y.z.A.B.C.D.E.F.G.H.I.J.uQtQt",
"QtQt.a.K.w.L.M.N.O.P.Q.Q.R.S.T.U.V.F.W.uQtQt",
"QtQt.b.i.X.M.Y.Z.P.0.1.2.3.P.Z.4.5.6.7.uQtQt",
"QtQt.b.j.y.8.9#.###a#b#c#d#e#f#g#h#i.W.uQtQt",
"QtQt.b.k.z#j#g#k#l#m#n#o#p#q#r.9#s#t#u.uQtQt",
"QtQt.b.l#v#w#x#a#y#z#A#B.9#C#D#E#g#F#G.uQtQt",
"QtQt.b.m#H#g#I#J#K#L#M.9#N.D#O#P#g#Q#R.uQtQt",
"QtQt.b.o#S.Q#T#U#V#B.9#.#W#X#Y#Z#g#0#1.uQtQt",
"QtQt.b.n#2.R#3#d#4.9#5#6#.#7#8#9a.a#aa.uQtQt",
"QtQt.b.mabac.Padaeafagahai.Qajakalaman.uQtQt",
"QtQt.baoapaq.ParasatauavawaxayazaAaBaC.uQtQt",
"QtQt.b.maDaEaF.Q.ZaGaHaIaJaKazazaLaMaN.uQtQt",
"QtQt.caO.GaPaQaRaS#.#g.Q#gaTaAaUaVaWaX.uQtQt",
"QtQt.d.saYaZa0a1a2a3a4a5a6a7a8a9b.b#ba.uQtQt",
"QtQt.e.t.JbbbcbdbebfbgbhbibjaCbkblbmbn.uQtQt",
"QtQtQt.u.u.u.u.u.u.u.u.u.u.u.u.u.u.u.uQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};


    switch (id) {
        case image0_ID: return QPixmap((const char**)image0_data);
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class ossimQtLayerChooser: public Ui_ossimQtLayerChooser {};
} // namespace Ui

class ossimQtLayerChooser : public QDialog, public Ui::ossimQtLayerChooser
{
    Q_OBJECT

public:
    ossimQtLayerChooser(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtLayerChooser();

public slots:
    virtual void theApplyButton_clicked();
    virtual void theCancelButton_clicked();

signals:
    void applySelected(ossimConnectableObject::ConnectableObjectList& selecteObjects);

protected slots:
    virtual void languageChange();

};

#endif // OSSIMQTLAYERCHOOSER_H
