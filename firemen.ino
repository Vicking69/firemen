  #include <math.h> 
#include <EEPROM.h>
#include <SPI.h> //imports the SPI library to communicate with Gamebuino's screen
#include <Gamebuino.h> //imports the Gamebuino library
Gamebuino gb;//creates a Gamebuino object named gb 

// sprites declaration ; they define in sprites.ino file
extern const byte casque[];
extern const byte building[];
extern const byte ambulance[];
extern const byte pompier[];
extern const byte suicide[]; 
extern const byte flame[]; 

// declaration coordinates pompier
float PX, PY;
int fire_size ;
// declaration coordinates suicide
float SX, SY;
int suic_size ;
float SXd , SYd  ;
// declaration global variables
int highscore ;
int score ;
int timer ;
int aff_flame1 , aff_flame2 ;

void setup()
{
  gb.begin();
  gb.titleScreen(F("FireMen"),casque);
  gb.battery.show = true;
  gb.backlight.set(255);
  gb.display.fontSize = 1;
      
  // initialisation variables function
  init_variables();
  
  // highscore valorisation
  if (EEPROM.read(0) == 0xff)
    EEPROM.put(0, 0x0000);
  else
    EEPROM.get(0, highscore);
}

void init_variables() // valorisation variables
{
PX=16; PY=40; fire_size=8 ;
SX=16; SY=0; suic_size=8 ;
SXd=0.5 ; SYd=1  ;
score=0 ; timer=1000; aff_flame1=10 ; aff_flame2 = 0 ;
}

void end_page_display()
{
    gb.display.cursorX = 15;
    gb.display.cursorY = 3;
    gb.display.println("GAME OVER !!!!");
       
    // highscore valorisation
    if (score > highscore)
    {
       //save highscore
       highscore = score;
       EEPROM.put(0, highscore);
       
    }
       gb.display.cursorX = 5 ;
       gb.display.cursorY = 10;
       gb.display.println("High score : ");
       gb.display.cursorX = 30;
       gb.display.println(highscore);
        
       gb.display.cursorX = 5 ;
       gb.display.println("score : ");
       gb.display.cursorX = 30;
       gb.display.println(score);
       
       gb.display.cursorX = 3;
       gb.display.cursorY = 35;
       gb.display.println("Press C - Exit");
       gb.display.println("Other touch - Replay");
    
    if(gb.buttons.pressed(BTN_C))
    {
       gb.titleScreen(F("FireMen"),casque);
    } 
    if(gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B))
    { init_variables ();}
}
void loop(){
  if(gb.update())
  {       
      
      if (timer==0) 
      {
        end_page_display() ;
      }
      else
      {
         //out of game if C is pressed
         if(gb.buttons.pressed(BTN_C))
         {
           end_page_display() ;
         }  
         else
         {
         // view the bulding 
         gb.display.drawBitmap(0,0,building);

         // view ambulance
         gb.display.drawBitmap(75,40,ambulance);      
         //end of time    
         if (aff_flame1 > 0 )
         { gb.display.drawBitmap(16,13,flame); gb.display.drawBitmap(10,33,flame); aff_flame1 = aff_flame1 - 1 ; }
         if (aff_flame1 == 1) {aff_flame2 = 10; aff_flame1 = 0 ; }
      
         if (aff_flame2 > 0 )
         { gb.display.drawBitmap(14,20,flame); gb.display.drawBitmap(16,8,flame); aff_flame2 = aff_flame2 - 1  ;}
         if (aff_flame2 == 1) {aff_flame1 = 10; aff_flame2 = 0 ; }
      
         // move and check position to fireman
         if(gb.buttons.repeat(BTN_RIGHT,2)){  //Si l'utilisateur reste appuyer sur la flÃ¨che de droite 
           if (PX!=67) { PX=PX+1.5 ; } else { PX=PX ;}
         }
         if(gb.buttons.repeat(BTN_LEFT,2)){  //Si l'utilisateur reste appuyer sur la flÃ¨che de gauche
           if (PX!=18)  { PX=PX-1.5 ;} else { PX=PX ; }
         }
         if(gb.buttons.repeat(BTN_DOWN,2)){} ; //Si l'utilisateur reste appuyer sur la flÃ¨che du bas
         if(gb.buttons.repeat(BTN_UP,2)){} ;  //Si l'utilisateur reste appuyer sur la flÃ¨che du haut
      
         gb.display.drawBitmap(PX,PY,pompier);

        
         // move and check position to suicide
         if (SX<75) { SX=SX+SXd ; } else { SX=8 ; SY=0 ; SYd=1 ;}
         if (SY>=40) { SX=8 ; SY=0 ; SYd=1 ; } else { SY=SY+SYd ; } 
         if (SY<=18) { SYd= 1 ; } 
         gb.display.drawBitmap(SX,SY,suicide);
    
         // rescue of fireman and suicide = collision
         if (gb.collideRectRect(SX, SY, suic_size, suic_size, PX, PY, fire_size, fire_size)) 
         {
             SYd=-1;
         }

         // rescue of abulance and suicide = collision
         if (gb.collideRectRect(SX, SY, suic_size, suic_size, 75, 40, 8, 8)) 
         {
            score=score+1 ;
            SX=16 ; SY=0 ; SYd=1 ;
            gb.sound.playTick();
            gb.sound.playTick();
         }
      
         // view score
         gb.display.cursorX=73; 
         gb.display.cursorY=1;
         gb.display.println(score);
      
         // view timer
         timer=timer-1;
         gb.display.cursorX=73; 
         gb.display.cursorY=7;
         gb.display.println(timer);
      
        

         //pause the game if B is pressed
         if (gb.buttons.pressed(BTN_B)) {
           gb.titleScreen(F("FireMen - Paused"),casque);
           }
        }
     }
  }
}
