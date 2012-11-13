#include "nesemulatordockwidget.h"
#include "ui_nesemulatordockwidget.h"

#include "cobjectregistry.h"
#include "main.h"

#include "emulatorprefsdialog.h"

#include "nes_emulator_core.h"

NESEmulatorDockWidget::NESEmulatorDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::NESEmulatorDockWidget)
{
   int32_t i;

   imgData = new char[256*256*4];

   ui->setupUi(this);

   QWidget* titleBar = titleBarWidget();
   QWidget* faker = new QWidget();
   faker->setMaximumHeight(0);
   setTitleBarWidget(faker);

   setMinimumSize(0,titleBarWidget()->sizeHint().height()+1);
   setMaximumSize(0,titleBarWidget()->sizeHint().height()+1);
   ncRect = rect();

   setTitleBarWidget(titleBar);
   setMinimumSize(0,0);
   setMaximumSize(16777215,16777215);
   delete faker;

   renderer = new CNESEmulatorRenderer(this,imgData);
   renderer->setMouseTracking(true);
   setWidget(renderer);//use this instead?

   m_joy [ CONTROLLER1 ] = 0x00;
   m_joy [ CONTROLLER2 ] = 0x00;

   // Clear image to set alpha channel...
   for ( i = 0; i < 256*256*4; i+=4 )
   {
      imgData[i] = 0;
      imgData[i+1] = 0;
      imgData[i+2] = 0;
      imgData[i+3] = 0xFF;
   }
   nesSetTVOut((int8_t*)imgData);
}

NESEmulatorDockWidget::~NESEmulatorDockWidget()
{
    delete ui;
    delete renderer;
    delete imgData;
}

void NESEmulatorDockWidget::setScalingFactor(float factor)
{
   QRect rect;
   if ( EmulatorPrefsDialog::get43Aspect() )
   {
      rect = ncRect.adjusted(0,0,factor*292,factor*240);
   }
   else
   {
      rect = ncRect.adjusted(0,0,factor*256,factor*240);
   }

   if ( factor > 1.0 )
   {
      setFloating(true);
   }
   else
   {
      setFloating(false);
   }
   setMinimumSize(rect.width(),rect.height());
   resize(rect.width(),rect.height());
}

void NESEmulatorDockWidget::updateTargetMachine(QString target)
{
   QObject* breakpointWatcher = CObjectRegistry::getObject("Breakpoint Watcher");
   QObject* emulator = CObjectRegistry::getObject("Emulator");

   if ( !target.compare("nes") )
   {
      QObject::connect(this,SIGNAL(controllerInput(uint8_t*)),emulator,SLOT(controllerInput(uint8_t*)));
   }
   QObject::connect(emulator, SIGNAL(emulatedFrame()), this, SLOT(renderData()));
   QObject::connect(breakpointWatcher, SIGNAL(breakpointHit()), this, SLOT(renderData()));
}

void NESEmulatorDockWidget::changeEvent(QEvent* e)
{
   QDockWidget::changeEvent(e);

   switch (e->type())
   {
      case QEvent::LanguageChange:
         ui->retranslateUi(this);
         break;
      default:
         break;
   }
}

void NESEmulatorDockWidget::mousePressEvent(QMouseEvent* event)
{
   if ( event->button()&Qt::LeftButton )
   {
      // Check for Zapper connected to mouse clicks.
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Zapper) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER1,IO_Zapper_FIRE)) )
      {
         m_joy [ CONTROLLER1 ] |= ZAPPER_TRIGGER;
      }
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Zapper) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER2,IO_Zapper_FIRE)) )
      {
         m_joy [ CONTROLLER2 ] |= ZAPPER_TRIGGER;
      }

      // Check for Vaus(Arkanoid) connected to mouse clicks.
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Vaus) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER1,IO_Vaus_FIRE)) )
      {
         m_joy [ CONTROLLER1 ] |= VAUS_FIRE;
      }
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Vaus) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER2,IO_Vaus_FIRE)) )
      {
         m_joy [ CONTROLLER2 ] |= VAUS_FIRE;
      }

      emit controllerInput ( m_joy );

      event->accept();
   }
}

void NESEmulatorDockWidget::mouseReleaseEvent(QMouseEvent* event)
{
   if ( event->button()&Qt::LeftButton )
   {
      // Check for Zappers connected to mouse clicks.
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Zapper) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER1,IO_Zapper_FIRE)) )
      {
         m_joy [ CONTROLLER1 ] &= (~(ZAPPER_TRIGGER));
      }
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Zapper) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER2,IO_Zapper_FIRE)) )
      {
         m_joy [ CONTROLLER2 ] &= (~(ZAPPER_TRIGGER));
      }

      // Check for Vaus(Arkanoid) connected to mouse clicks.
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Vaus) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER1,IO_Vaus_FIRE)) )
      {
         m_joy [ CONTROLLER1 ] &= (~(VAUS_FIRE));
      }
      if ( (EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Vaus) &&
           (EmulatorPrefsDialog::getControllerMouseMap(CONTROLLER2,IO_Vaus_FIRE)) )
      {
         m_joy [ CONTROLLER2 ] &= (~(VAUS_FIRE));
      }

      emit controllerInput ( m_joy );

      event->accept();
   }
}

void NESEmulatorDockWidget::keyPressEvent(QKeyEvent* event)
{
   if ( EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_StandardJoypad )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_LEFT) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_LEFT;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_RIGHT) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_RIGHT;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_UP) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_UP;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_DOWN) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_DOWN;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_SELECT) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_SELECT;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_START) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_START;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_B) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_B;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_A) )
      {
         m_joy [ CONTROLLER1 ] |= JOY_A;
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Zapper )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_Zapper_FIRE) )
      {
         m_joy [ CONTROLLER1 ] |= ZAPPER_TRIGGER;
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Vaus )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_Vaus_FIRE) )
      {
         m_joy [ CONTROLLER1 ] |= VAUS_FIRE;
      }
   }

   if ( EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_StandardJoypad )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_LEFT) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_LEFT;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_RIGHT) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_RIGHT;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_UP) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_UP;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_DOWN) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_DOWN;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_SELECT) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_SELECT;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_START) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_START;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_B) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_B;
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_A) )
      {
         m_joy [ CONTROLLER2 ] |= JOY_A;
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Zapper )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_Zapper_FIRE) )
      {
         m_joy [ CONTROLLER2 ] |= ZAPPER_TRIGGER;
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Vaus )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_Vaus_FIRE) )
      {
         m_joy [ CONTROLLER2 ] |= VAUS_FIRE;
      }
   }

   emit controllerInput ( m_joy );

   event->accept();
}

void NESEmulatorDockWidget::keyReleaseEvent(QKeyEvent* event)
{
   if ( EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_StandardJoypad )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_LEFT) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_LEFT);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_RIGHT) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_RIGHT);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_UP) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_UP);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_DOWN) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_DOWN);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_SELECT) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_SELECT);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_START) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_START);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_B) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_B);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_StandardJoypad_A) )
      {
         m_joy [ CONTROLLER1 ] &= (~JOY_A);
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Zapper )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_Zapper_FIRE) )
      {
         m_joy [ CONTROLLER1 ] &= (~ZAPPER_TRIGGER);
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER1) == IO_Vaus )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER1,IO_Vaus_FIRE) )
      {
         m_joy [ CONTROLLER1 ] &= (~VAUS_FIRE);
      }
   }

   if ( EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_StandardJoypad )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_LEFT) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_LEFT);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_RIGHT) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_RIGHT);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_UP) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_UP);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_DOWN) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_DOWN);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_SELECT) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_SELECT);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_START) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_START);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_B) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_B);
      }
      else if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_StandardJoypad_A) )
      {
         m_joy [ CONTROLLER2 ] &= (~JOY_A);
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Zapper )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_Zapper_FIRE) )
      {
         m_joy [ CONTROLLER2 ] &= (~ZAPPER_TRIGGER);
      }
   }
   else if ( EmulatorPrefsDialog::getControllerType(CONTROLLER2) == IO_Vaus )
   {
      if ( event->key() == EmulatorPrefsDialog::getControllerKeyMap(CONTROLLER2,IO_Vaus_FIRE) )
      {
         m_joy [ CONTROLLER2 ] &= (~VAUS_FIRE);
      }
   }

   emit controllerInput ( m_joy );

   event->accept();
}

void NESEmulatorDockWidget::renderData()
{
   renderer->update();
}
