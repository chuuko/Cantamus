#include "pianoroll.h"
#include "handleust.h"
#include <QPen>
#include <QtWidgets>
#include <QMessageBox>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

/*
Piano roll interface. Needs piano marquee
*/

const int C4 = 83;
unsigned int num1;
unsigned int cur;
int selNoteIsValid=0;
int indicator = 0;
QPushButton *okButton;
int notex = 0;
std::vector<int> storeNotex;
QRectF noteRect;
QImage noteImage;
QLineEdit *lirEdit;
QRectF curNote;
QRectF selection;
int selectionSwitch=0;
int mouseLeftIsHeld=0;
int abcPos=0;
int abc=0;
int abcCount;


ustHandler *cFileHandler;
int storedWidth=1080;
int extra =0;

pianoRoll::pianoRoll(QWidget *parent):
    QWidget(parent)

{
    storeNotex.resize(10000);
    storeNotex.reserve(10000);
    makeRest.resize(10000);
    actualnotelength.resize(10000);
    actualnotelength.reserve(10000);
    actualnotepitch.resize(10000);
    actualnotepitch.reserve(10000);
    noteS.resize(10000);
    fromOpen = false;
    afterOpen = false;
    pitchLineLength=2000;
    setGeometry(0,0,2000,2640);
    setBaseSize(2000,2640);
    setMinimumSize(2000,2640);

    //setMaximumSize(3000,2640);
    //setSizeIncrement(2360,0);
    selectMode();

    noteLast=false;
    noteFb=false;

    /*wPosL = new QLabel(QString::number(winPos));
    wPosL->setGeometry(100,100,100,100);
    wPosL->show();*/

    acceptDrops();
    actualnotecount=0;
    actualnotelyrics.resize(10000);
    noteLengths=0;
    setUpdatesEnabled(true);
    setMouseTracking(true);
    modCorr=0;
    modCorr2=0;
    undoCodes.resize(10000);
    selArray.resize(10000);
    std::cout << undoCodes.length() <<"\n";
    selArray.fill(false);
    noteS.fill(0);
}

void pianoRoll::paintEvent(QPaintEvent*)
{
   int x=0;
   int y=0;
   int c=-5;
   int cplace=235;
   int cnum = 10;

   QPoint clabel(5,c);
   QPoint numlabel(15,cplace);
   QPoint beginline(x,0);

   QPoint endline(x,2640);

   QPoint beginline2(0,y);

   QPoint endline2(pitchLineLength,y);

   //cFileHandler = new ustHandler();

   QPainter painter(this);
   QPen pen(QColor(0,0,0,255));
   QBrush brush(Qt::SolidPattern);
   brush.setColor(QColor(255,255,255,255));
   pen.setStyle(Qt::SolidLine);
   painter.setPen(pen);
   painter.setBrush(brush);
   painter.setFont(QFont("Helvetica"));
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

   painter.drawRect(QRect(0,0,pitchLineLength,2640));


   for (count=0;count<17+extra;count++)
   {
       painter.drawLine(beginline,endline);

       beginline.setX(x+120);
       endline.setX(x+120);

       x = x+120;
   }

   for (noteline=0; noteline<=142;noteline++)
   {
       painter.drawLine(beginline2,endline2);

       beginline2.setY(y+20);
       endline2.setY(y+20);

       y=y+20;
   }

   for (fonty=0; fonty<=11; fonty++)
   {
       painter.drawText(clabel,QString("C"));
       clabel.setY(c+240);
       c=c+240;
       cnum = cnum-1;

   }

   //For some reason, it won't draw number strings
   painter.drawText(numlabel,QString("10"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("9"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("8"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("7"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("6"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("5"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("4"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("3"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("2"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("1"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;
   painter.drawText(numlabel,QString("0"));
   numlabel.setY(cplace+240);
   cplace = cplace+240;

   p=0;

   std::cout << "a" << std::endl;
   for (noteCount=0; noteCount < actualnotecount; noteCount++)
   {
       noteImage = QImage((actualnotelength[p]/4),20,QImage::Format_ARGB32_Premultiplied);
       noteImage.fill(qRgba(0,0,0,0));
       brush.setStyle(Qt::SolidPattern);

       std::cout << noteS[p] << "\n";
       noteRect=QRectF(notex+noteS[p],(2640-(actualnotepitch[p]*20)-240),(actualnotelength[p]/4),20);

       if(makeRest[p]=0)
       {
           std::cout << "Rest note" <<std::endl;
       }

       else
       {
           painter.drawRect(noteRect);
           if(noteRect==curNote)
           {
               if(selNoteIsValid==1)
               {
                   brush.setColor(Qt::yellow);
                   propLyric=QString(actualnotelyrics[p]);
                   propLength=actualnotelength[p];
                   //pToSend2=p;
               }
           }

           else
           {
               brush.setColor(QColor(255,154,203,255));
           }

           painter.fillRect(noteRect,brush);
           painter.drawText(noteRect,QString(actualnotelyrics[p]));
           painter.drawPixmap(notex+noteS[p],(2640-(actualnotepitch[p]*20)-240),QPixmap::fromImage(noteImage));

       }


       storeNotex[p]=notex;
       notex= notex+(actualnotelength[p]/4);
       noteLengths=noteLengths+(actualnotelength[p]);
       lstore=noteLengths/4;
       p=p+1;

   }
   /*for (numy=0; numy>=10; numy++)
   {
       painter.drawText(numlabel,QString("10"));
       numlabel.setY(cplace+240);
       cplace=cplace+240;
       cnum=cnum-1;
   }*/

   notex=0;

   if(selectionSwitch==1)
   {
       brush.setColor(QColor(255,130,0,50));
       painter.setCompositionMode(QPainter::CompositionMode_Multiply);
       painter.drawRect(selection);
       painter.fillRect(selection,brush);
       update();
   }
   //QMessageBox::warning(this,"foo",QString::number(pitchLineLength)+" "+QString::number(noteLengths));
}

void pianoRoll::moveEvent(QMoveEvent*)
{


}
void pianoRoll::selectMode()
{
    setCursor(QCursor(Qt::PointingHandCursor));
}
void pianoRoll::drawMode()
{
    setCursor(QCursor(Qt::CrossCursor));
}
void pianoRoll::eraseMode()
{
    setCursor(QCursor(Qt::UpArrowCursor));
}
void pianoRoll::showProperties()
{
    properties = new QDialog(this);
    properties->setBaseSize(200,200);
    properties->setMinimumSize(200,200);
    properties->setSizeIncrement(200,2);
    properties->setMaximumSize(400,400);

    propLayout = new QGridLayout(properties);
    propLayout->setAlignment(propLayout,Qt::AlignJustify);

    properties->setLayout(propLayout);

    lengthLabel2 = new QLabel(properties);
    lyricLabel2 = new QLabel(properties);
    lengthLabel2->setText("Length");
    lyricLabel2->setText("Lyrics");

    lyricarea = new QLineEdit(properties);
    lengtharea = new QLineEdit(properties);
    lyricarea->setText(propLyric);
    lengtharea->setText(QString::number(propLength));

    propButs.setStandardButtons(QDialogButtonBox::Ok |QDialogButtonBox::Cancel);

    propLayout->addWidget(lengthLabel2,0,0);
    propLayout->addWidget(lengtharea,0,1);
    propLayout->addWidget(lyricLabel2,1,0);
    propLayout->addWidget(lyricarea,1,1);
    propLayout->addWidget(&propButs,2,0);

    properties->setWindowTitle("Note properties");
    properties->setLayoutDirection(Qt::LeftToRight);
    properties->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    properties->show();

    connect(&propButs,SIGNAL(accepted()),this,SLOT(finishLirChange2()));
    connect(&propButs,SIGNAL(rejected()),properties,SLOT(close()));

}
void pianoRoll::noteMake()
{
    //QMessageBox::warning(this,"a",QString::number(cur) + " " + QString::number(num1) + " " + QString::number(noteLengths) + " " + QString::number(length1) + " " + QString::number(inputNoteLength) + " " + lyricInput + " " + QString::number(inputNotePitch) + " " + QString::number(scrollAmount));

    actualnotecount=actualnotecount+1;

    actualnotelength[p]=inputNoteLength;

    makeRest[p] = 1;

    if (noteLast==true)
    {
        noteFb=true;
    }

    if (lyricInput.isEmpty())
    {
        lyricInput=QString("a");
    }

    actualnotelyrics[p]=lyricInput;
    actualnotepitch[p]=inputNotePitch;
    std::cout <<  inputNotePitch <<"\n";

    selArray[p]=0;


    //QMessageBox::warning(this,"note","Note length "+QString::number(inputNoteLength)+" "+QString::number(num1)+" "+QString::number(lstore)/*+" "+QString::number(changePrecedingNoteTrackIfNeeded)*/+" "+QString::number(cur));

    update();


    notex=0;

    evaluate();

    update();

    if(noteFb==true)
    {
        noteLast=false;
        noteFb=false;
        abcCount=0;
        abc=0;
        abcPos=0;
    }

    //selArray.resize(actualnotecount);


    addUndo(0);

}

void pianoRoll::evaluate()
{
    if (fromOpen==true)
    {
        setMinimumWidth(lstore/4);
        pitchLineLength=lstore/4;
        extra=extra+(lstore/12);
        fromOpen = false;
    }
    else
    {


        if (lstore>=storedWidth)
        {
            if (afterOpen==true)
            {
                storedWidth=lstore;
            }

            setMinimumWidth(pitchLineLength+960);
            pitchLineLength=pitchLineLength+960;
            extra=extra+9;
            storedWidth=storedWidth+840;
            fromOpen = false;
            afterOpen = false;
        }
    }
    actNote = 0;

}


void pianoRoll::clearRoll()
{
    actualnotecount=0;
    p=0;
    lstore=0;
    noteLengths=0;
    pitchLineLength=2000;
    setMinimumWidth(pitchLineLength);
    storedWidth=1080;
    notex=0;
    noteS.clear();
    noteS.resize(10000);
    noteS.reserve(10000);
    abc=0;
    abcPos=0;
    modCorr=0;
    lirChangeI=0;

    lyricInput.clear();
    lyricInput.resize(0);
    lyricInput.reserve(10000);
    //-------------------------
    update();
    u = 0;
    undoCodes.clear();
    undoCodes.resize(10000);
    undoCodes.reserve(10000);

}
void pianoRoll::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(cursor().shape()==Qt::PointingHandCursor)
    {
        if(event->y()<(2640-(actualnotepitch[p]*20)-240))
        {
            event->ignore();
        }
        else
        {
             if(selNoteIsValid==1)
              {
                lirEdit=new QLineEdit(this);
                lirEdit->setGeometry(curNote.x(),curNote.y(),curNote.width(),curNote.height());
                lirEdit->setText(actualnotelyrics[pToSend]);
                lirEdit->show();
                indicator=1;
              }
        }
        if(event->y()>(2640-(actualnotepitch[p]*20)-220))
        {
            event->accept();
        }
        else
        {

            if(selNoteIsValid==1)
            {
                lirEdit=new QLineEdit(this);
                lirEdit->setGeometry(curNote.x(),curNote.y(),curNote.width(),curNote.height());
                lirEdit->setText(actualnotelyrics[pToSend]);
                lirEdit->show();
                indicator=1;
            }
        }
    }
}
void pianoRoll::selectRect(QPoint mousePos)
{
    selection=QRectF(mousePos.x(),mousePos.y(),0,0);
    corner.setX(mousePos.x());
    corner.setY(mousePos.y());
    selectionSwitch=1;
}
void pianoRoll::mousePressEvent(QMouseEvent *event)
{
    if (cursor().shape()==Qt::CrossCursor)
    {
        cur=event->x();
        int pitch1 = pitchFromPos(event->localPos().y());
        if(pitch1>(12*11))
        {
            pitch1 = pitchFromPos(event->localPos().y());
        }
        if(cur+scrollAmount>lstore)
        {

            num1 = quantizeMultiplier(event->x());

            length1 = (quantizeMultiplier(event->x())*quantizeValue);
            abcCount=lyricRouter.size();

            if (noteLengths>length1)
            {
                noteLengths=0;
                p=0;

                for(int derp=0;derp<actualnotecount;++derp)
                {
                    noteLengths=noteLengths+(actualnotelength[p]);
                    p=p+1;
                }
            }

            length1 = (quantizeMultiplier(event->x())*quantizeValue);
            //wPosL->setText(QString::number(noteLengths));
            //wPosL->update();
            if(modCorr2==0)
            {
                inputNoteLength = length1-(noteLengths);
            }
            if(modCorr2>0)
                {
                    inputNoteLength = length1-(noteLengths)-modCorr+quantizeValue;
                }

            if(modCorr2<0)
                {
                    inputNoteLength = length1-(noteLengths)-modCorr;
                    modCorr=0;
                }



            for(abc=abc;abc<abcCount;abc++)
            {
                if(lyricRouter.mid(abc,1)==QString(","))
                {
                    if(inputNoteLength>=quantizeValue || cur+scrollAmount>lstore)
                    {

                        lyricInput=lyricRouter.mid(abcPos,abc-abcPos);
                        abcCount=abc;
                        abcPos=abcCount+1;
                        abc=abcPos;
                    }

                    else
                    {
                        abcCount=abc;
                        abcPos=abcCount-1;
                        abc=abcPos;
                    }

                }

            }

            if(abc>lyricRouter.size())
            {
                noteLast=true;

            }

            inputNotePitch=pitch1;

        }

    }
    else if(cursor().shape()==Qt::PointingHandCursor)
    {

        if(indicator == 1)
        {}
        else
        {
            noteLengths = 0;
        }
        curNote  = targetRect1(event->pos());
        p=0;

        for(curNote.x();curNote.x()>storeNotex[p]+(actualnotelength[p]/4);)
        {
         p++;
        }

        if(event->y()<(2640-(actualnotepitch[p]*20)-240))
        {
            selNoteIsValid=0;
            update();
            if(indicator==1)
            {
                isFromButton=1;
                finishLirChange();
            }
            else
            {

            }

            selectRect(event->pos());
        }

        else
        {
            if(event->y()>(2640-(actualnotepitch[p]*20)-220))
            {
                selNoteIsValid=0;
                update();
                if(indicator==1)
                {
                    isFromButton=1;
                    finishLirChange();
                }

                else
                {
                }

                selectRect(event->pos());

            }
        }

        if(curNote.y()<(2640-(actualnotepitch[p]*20)-240))
        {

        }

        else
        {
            if(curNote.y()>(2640-(actualnotepitch[p]*20)-220))
            {
            }

            else
            {
                curNote=QRectF(storeNotex[p]+noteS[p],(2640-(actualnotepitch[p]*20)-240),actualnotelength[p]/4,20);
                pToSend=p;
                selNoteIsValid=1;
                mouseLeftIsHeld=1;
                update();

                event->accept();

            }
        }
    }
    else if (cursor().shape()==Qt::UpArrowCursor)
    {
        curNote  = targetRect1(event->pos());
        p=0;

        for(curNote.x();curNote.x()>storeNotex[p]+(actualnotelength[p]/4);)
        {
            p++;
        }

        //QMessageBox::warning(this,"",QString::number(event->y())+" "+QString::number(2640-(actualnotepitch[p]*20)-20-210)+" "+QString::number(2640-(actualnotepitch[p]*20)-210));
        if(event->y()>(2640-(actualnotepitch[p]*20)-20-210)&&event->y()<(2640-(actualnotepitch[p]*20)-210))
        {
        curNote=QRectF(storeNotex[p]+noteS[p],(2640-(actualnotepitch[p]*20)-240),actualnotelength[p]/4,20);

        pToSend=p;
        deleteNote();
        event->accept();
        }
    }

}

void pianoRoll::mouseMoveEvent(QMouseEvent *event)
{
   if(cursor().shape()==Qt::PointingHandCursor){

       if(selectionSwitch==1)
       {
           growSelect(event->pos());
       }

       if(selNoteIsValid==1)
       {
           if(mouseLeftIsHeld==1)
           {
               actualnotepitch[pToSend]=pitchFromPos(event->y());
               mn = 0;
               while(makeRest[pToSend-mn]==0)
               {
                   mn++;
                   if(pToSend-mn<0)
                   {
                       mn = 0;
                       break;
                   }
               }

               if(event->x()-storeNotex[pToSend-mn]>=0)
               {
                   noteS[pToSend]=event->x()-storeNotex[pToSend];
                   std::cout << noteS[pToSend] << "\n";
               }
               //notex=event->x();
               if(pToSend !=0)
               {
                    if(makeRest[pToSend-1]==1)
                    {
                        actualnotelength[pToSend-1]=actualnotelength[pToSend-1]+noteS[pToSend];
                    }
               }
               curNote=QRectF(storeNotex[pToSend]+noteS[pToSend],(2640-(actualnotepitch[pToSend]*20)-240),actualnotelength[pToSend]/4,20);
               update();
               addUndo(2);
               mn = 0;
               event->accept();
           }

       }
   }
}

void pianoRoll::growSelect(QPoint growth)
{
   selection=QRectF(corner.x(),corner.y(),growth.x()-corner.x(),growth.y()-corner.y());
   update();
}

void pianoRoll::showMouseMenu(QPoint mousePos)
{
    rightClickMenu = new QMenu(this);
    //rightClickMenu->setContextMenuPolicy(Qt::ActionsContextMenu);
    //rightClickMenu->pos()=QPoint(200,200);
    //rightClickMenu->x()=200;
    rightClickMenu->setGeometry(mousePos.x(),mousePos.y(),100,100);//130
    actionProp = new QAction("Note Properties",this);
    actionDel = new QAction("Delete",this);
    rightClickMenu->addAction(actionProp);
    rightClickMenu->addAction(actionDel);
    connect(actionProp,SIGNAL(triggered()),this,SLOT(showProperties()));
    connect(actionDel,SIGNAL(triggered()),this,SLOT(del()));
    //QMessageBox::warning(this,"a",QString::number(scrollCor_x));
    rightClickMenu->update();
    rightClickMenu->show();

}
void pianoRoll::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button()==Qt::RightButton)
    {
        if(selNoteIsValid==1)
           {
            showMouseMenu(event->globalPos());
            event->accept();
        }
    }
    if (cursor().shape()==Qt::CrossCursor)
    {
        if (inputNoteLength==0)
        {

            /*if (lirChangeI==1)
            {
                if(cur+scrollAmount>lstore)
                {

                    num1 = quantizeMultiplier(event->x());

                    length1 = (quantizeMultiplier(cur)*quantizeValue);
                    abcCount=lyricRouter.size();



                    if (noteLengths>length1)
                    {
                        noteLengths=0;
                        p=0;
                        for(int derp=0;derp<actualnotecount;++derp)
                            {
                                noteLengths=noteLengths+(actualnotelength[p]);
                                p=p+1;
                            }
                    }

                    length1 = (quantizeMultiplier(event->x())*quantizeValue);

                    if(modCorr2==0)
                    {
                        inputNoteLength = length1-(noteLengths);
                    }
                    if(modCorr2>0)
                        {
                            inputNoteLength = length1-(noteLengths)+modCorr-quantizeValue;
                        }

                    if(modCorr2<0)
                        {
                            inputNoteLength = length1-(noteLengths)-modCorr+quantizeValue;
                            modCorr=0;
                        }



                    for(abc=abc;abc<abcCount;abc++)
                    {
                        if(lyricRouter.mid(abc,1)==QString(","))
                        {
                            if(inputNoteLength>=quantizeValue || cur+scrollAmount>lstore)
                            {

                                lyricInput=lyricRouter.mid(abcPos,abc-abcPos);
                                abcCount=abc;
                                abcPos=abcCount+1;
                                abc=abcPos;
                            }
                            else
                            {
                                abcCount=abc;
                                abcPos=abcCount-1;
                                abc=abcPos;
                            }
                        }
                    }
                    if(abc>lyricRouter.size())
                    {
                        noteLast=true;

                    }
                }

            }*/
            noteLengths=0;
        }

        //Check note pitch to prevent the ghost note glitch

        if(inputNotePitch>11*22)
        {
            inputNotePitch = pitchFromPos(event->y());
        }
        noteMake();

        noteLengths=0;
        length1=0;
        lirChangeI=0;
        inputNoteLength=0;
        quantizeMultiplier(0);

        event->accept();
    }
    else if(cursor().shape()==Qt::PointingHandCursor)
    {
        if(selectionSwitch==1)
        {
            selectionSwitch=0;
            noteLengths = 0;
            update();
        }
        if(selNoteIsValid==1)
        {

            mouseLeftIsHeld=0;
            notex = 0;
        }


        else
        {
            noteLengths = 0;
        }
    }
}
void pianoRoll::dragEnterEvent(QDragEnterEvent *event)
{

}
void pianoRoll::dragLeaveEvent(QDragLeaveEvent *event)
{}
void pianoRoll::dragMoveEvent(QDragMoveEvent *event)
{}
void pianoRoll::dropEvent(QDropEvent *event)
{}
QRectF pianoRoll::targetRect1 (QPointF position)
{
    return QRectF(position.x(), position.y(),actualnotelength[p]/4,20);
}
int pianoRoll::y()
{
    return (2640-(actualnotepitch[p]*20)-240);
}
int pianoRoll::noteSize()
{
    //return noteRect.width();
}
int pianoRoll::pitchFromPos(int posToConvert)
{
    /*posToConvert=(posToConvert+240+260);
    if (posToConvert % 20 !=0)
        posToConvert=(posToConvert)*/
    std::cout<<posToConvert<< " squeek " << ((2420-posToConvert)/20)<<std::endl;
    return ((2420-posToConvert)/20);
}
unsigned int pianoRoll::quantizeMultiplier(unsigned const int xToConvert) const
{

    unsigned const int length2 = ((xToConvert*4)/quantizeValue);//Scrolling ruins everything
    if (length2>0)
    {
        return length2;
        //length2=0;
    }
}
void pianoRoll::finishLirChange()
{
    if(isFromButton=0)
    {
    }
    else
    {
        actualnotelyrics[pToSend]=lirEdit->text();
        lirEdit->hide();
        update();
        indicator=0;
    }
}
void pianoRoll::finishLirChange2()
{
    if(selNoteIsValid==1)
    {
        if(properties->isVisible()==true)
        {
            lyricarea->update();
            //pToSend2=pToSend;
            dLyric=lyricarea->text();
            actualnotelyrics.replace(pToSend,dLyric);
            dLyric3=actualnotelyrics[0];
            dLyric2=actualnotelyrics[1];
            modCorr = abs(actualnotelength[pToSend]-lengtharea->text().toInt(0));
            modCorr2 = actualnotelength[pToSend]-lengtharea->text().toInt(0);
            noteLengths = noteLengths - modCorr2;

            while(modCorr>=quantizeValue)
            {
                modCorr = modCorr-quantizeValue;
            }
            actualnotelength[pToSend]=lengtharea->text().toInt(0);
            update();
            properties->close();
            if(pToSend==1)
            {
                actualnotelyrics[0]=dLyric3;
                dLyric2=dLyric;
                pToSend++;
            }
            if(pToSend==0)
            {
                pToSend++;
                actualnotelyrics.replace(1,dLyric);
                dLyric3=dLyric;

                update();
            }
            else
            {
                pToSend=0;
            }

        }
        else
        {
            actualnotelyrics[pToSend]=lyricInput;
            update();
        }
        if(actualnotelyrics.at(1)!=dLyric2)
        {
            //QMessageBox::warning(this,"a",dLyric2);
            actualnotelyrics[1]=dLyric2;
            update();
        }
        if(actualnotelyrics.at(0)!=dLyric3)
        {
            //
            actualnotelyrics[0]=dLyric3;
            update();
        }
        //QMessageBox::warning(this,"a",QString::number(inputNoteLength));

    }
    update();

    lirChangeI = 1;

}
void pianoRoll::changesLength()
{

}
void pianoRoll::deleteNote()
{
    if(cursor().shape()==Qt::UpArrowCursor||deleteActNote == true)
    {

        //lstore = lstore - actualnotelength[pToSend];
        //storedWidth = storedWidth - actualnotelength[pToSend];
        actualnotelyrics.removeAt(pToSend);
        update();

        for(pToSend;pToSend<actualnotecount;pToSend++)
        {
            int recPitch=actualnotepitch[pToSend+1];
            int recLength=actualnotelength[pToSend+1];
            actualnotelength[pToSend]=recLength;
            actualnotepitch[pToSend]=recPitch;
            update();
        }

        actualnotecount=actualnotecount-1;
        noteLengths = 0;

        update();
    }
    deleteActNote = false;
    addUndo(1);

}

void pianoRoll::del()
{
    deleteActNote = true;
    deleteNote();
}
void pianoRoll::addUndo(int code)
{
    undoCodes.insert(u+1,code);
    u++;
}
