#include "serialcomm.h"
#include "QSerialPort"
#include "QTextCodec"
#include <stdlib.h>
QSerialPort *serial;

#define AUTOMODE_RANDOM_DELAY  10 //100*50 = 5000ms
#define DOUBLETAP_DURATION     10  // 10*50 = 500ms
QString ArduinoSerialData;
int DoubleTab = 0;
bool AUTO_MODE = true;

//Initialize serial port
serialComm::serialComm(QObject *parent) : QObject(parent)
{
   serial = new QSerialPort(this);
   serial->setPortName("/dev/tty.usbmodem1411");
   serial->setBaudRate(QSerialPort::Baud9600);
   serial->setDataBits(QSerialPort::Data8);
   serial->setParity(QSerialPort::NoParity);
   serial->setStopBits(QSerialPort::OneStop);
   serial->setFlowControl(QSerialPort::NoFlowControl);
   serial->open(QIODevice::ReadWrite);
   connect(serial,SIGNAL(readyRead()),this,SLOT(serialDataReceived()));
}

QString serialComm::readSerial(bool Mode)
{
   QString returnValue;
   AUTO_MODE = Mode;
   int i = 0;

   if(AUTO_MODE == true){
       static int randomCounterDelay =0;
       if(randomCounterDelay==AUTOMODE_RANDOM_DELAY)
       {
              if(DoubleTab)
              {
                  i = 5;
              }
              else
              {
                  i =  rand() %10;
              }
              if(i==5) DoubleTab = 1;

              randomCounterDelay = 0;
        }
       else
       {
           randomCounterDelay++;
       }
   }
   else
   {
       i = ArduinoSerialData.toInt();
       ArduinoSerialData.clear();

   }
   // qDebug()<<i;

   switch(i)
   {
   case 1:
       returnValue = "ZoomIn"; //
       break;
   case 2:
       returnValue = "NavUp";
       break;
   case 3:
       returnValue = "ZoomOut";
       break;
   case 4:
       returnValue = "NavLeft";
       break;
   case 5:
       if(DoubleTab)
       {
           DoubleTab = 0;
           returnValue = "NavOverlay";
       }
       else
       {
          DoubleTab = 1;
       }
       break;
   case 6:
       returnValue = "NavRight";
       break;
   case 8:
       returnValue = "NavDown";
       break;
   case 9:
       returnValue = "ClearOverlay";
       break;
   case 7: //Not used
   case 0: //Not used
   default:
       break;
    }

   if(DoubleTab)
   {
       static int counter = 0;
       if(counter == DOUBLETAP_DURATION)
       {
           counter = 0;
           DoubleTab = 0;
       }
       else
       {
           counter++;
       }
    }
    return returnValue;
}


void serialComm::serialDataReceived()
{
    if(ArduinoSerialData == "")
    {
        ArduinoSerialData = serial->readAll();
    }
    else
    {
        //qDebug()<<"Missed messages!!";
    }

}
