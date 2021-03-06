//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.hpp"
#include <vector>
using namespace sf;

class card{// The class
  public:             // Access specifier
    int multiplicity=3; //(1,2,3) --> (0,1,2)
    int color=3;// (Red,Green,Purple) --> (0,1,2)
    int fill=3;  // (empty,striped,solid) --> (0,1,2)
    int shape=3;// (diamond,squiggle,oval) --> (0,1,2)
    //for reference our 3 red striped diamonds is (2,0,1,0)
    //two more cards 1 green solid oval (0,1,2,2)
    //the set would be completed by 2 purple empty squiggles (1,2,0,1)
    Texture style;//load the cards look into here
    bool played=false;
    //RectangleShape cardRectSize;
    Sprite look;
};

bool cardsAreEqual(card a,card b){
    if(b.multiplicity==a.multiplicity&&b.color==a.color&&b.fill==a.fill&&b.shape==a.shape)
        return true;
    else return false;
}

card thirdcard(card one, card two){
    card three;
    card empty;
    if(cardsAreEqual(one,two)) return empty;
    if(one.multiplicity!=two.multiplicity){
    three.multiplicity=3-(one.multiplicity+two.multiplicity);
    }
    else three.multiplicity=one.multiplicity;
    if(one.color!=two.color){
    three.color=3-(one.color+two.color);
    }
    else three.color=one.color;
    if(one.fill!=two.fill){
    three.fill=3-(one.fill+two.fill);
    }
    else three.fill=one.fill;
    if(one.shape!=two.shape){
    three.shape=3-(one.shape+two.shape);
    }
    else three.shape=one.shape;
    return three;
}

void dealacard(card deck[3][3][3][3],int *cardsleft,card onthefield[21],int *numonfield,bool cardremoved,int index,std::vector<int> &randomNumbers){
    int rnSize = randomNumbers.size();
    int a=rand()%rnSize;
    int random = randomNumbers[a];
    std::cout<<rnSize<<" "<<random<<" ";
    //select rand()%length of vector. use that random number as an index for the number stored at that point in the vector. do everything with the stored number not the random number and remove the stored number from the vector.
    int b[4]={0};
    for(int i=3;i>=0;i--){
        b[i]=random%3;
        random = random/3;
    }
    //while(deck[b[0]][b[1]][b[2]][b[3]].played==true){//can change this to linkedlist make game a bit faster
    //       a=rand()%81;
    //    for(int i=3;i>=0;i--){
    //        b[i]=a%3;
    //       a = a/3;
    //    }
    //    std::cout<<" oops ";
   // }
    deck[b[0]][b[1]][b[2]][b[3]].played=true;
    *cardsleft -= 1;
    randomNumbers.erase(randomNumbers.begin()+a);
    if(!cardremoved){
    onthefield[*numonfield]=deck[b[0]][b[1]][b[2]][b[3]];
        onthefield[*numonfield].look.setPosition(200*(*numonfield%4),200*(*numonfield/4));
    *numonfield += 1;
    }
    if(cardremoved){
        onthefield[index]=deck[b[0]][b[1]][b[2]][b[3]];
    }
    return;
}

void dealthreecards(card deck[3][3][3][3],int *cardsleft,card onthefield[21],int *numonfield,bool cardremoved,int index,std::vector<int> &randomNumbers){
        dealacard(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
        dealacard(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
        dealacard(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
        return;
}



bool setisfound(card a[3]){
    card b=thirdcard(a[0],a[1]);
    if(cardsAreEqual(b,a[2]))
     return true;
    else return false;
}

bool setexists(card onthefield[21],int numonfield,int *setsOnBoard,int *d,int *b,int *c){
    int somenumber=0;
    card a[3];
    for(int i=0;i<numonfield-2;i++){
        for(int j=i+1;j<numonfield-1;j++){
            for(int k=j+1;k<numonfield;k++){
                a[0]=onthefield[i];
                a[1]=onthefield[j];
                a[2]=onthefield[k];
        //if(setisfound(a)==true)count++;//cardsAreEqual(a[0],a[1])==false&&cardsAreEqual(a[1],a[2])==false&&cardsAreEqual(a[0],a[2])==false&&
        if(setisfound(a)==true){
            somenumber++;
            *d=i;
            *b=j;
            *c=k;
            }
            }
    }
    }
    *setsOnBoard=somenumber;
    if(somenumber>0) return true;
    else return false;
}


void startgame(card deck[3][3][3][3],int *cardsleft,card onthefield[21],int *numonfield,bool cardremoved,int index,int *setsOnBoard,int *d,int *b,int *c,std::vector<int> &randomNumbers){
    dealthreecards(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
    dealthreecards(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
    dealthreecards(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
    dealthreecards(deck,cardsleft,onthefield,numonfield,false,index,randomNumbers);
    for(int i=0;i<12;i++){
        onthefield[i].look.setPosition(200*(i%4),200*(i/4));
        }
    while(!setexists(onthefield,*numonfield,setsOnBoard,d,b,c)){
        dealthreecards(deck,cardsleft,onthefield,numonfield,false,0,randomNumbers);
    }
}

void itisaset(card deck[3][3][3][3],int *cardsleft,card onthefield[21],int *numonfield,int indexofselected[3],card selectarray[3],int *setsOnBoard,int *score,int *d,int *b,int *c,std::vector<int> &randomNumbers){
    int replace[3];
    card replacementCard[3];
    int thatone=0;
    int thatotherone=0;
    card empty;
    //std::cout<<empty.multiplicity<<empty.color<<empty.fill<<empty.shape<<std::endl;
    *score += 1;
    int temp=*numonfield;
    if(*numonfield>12||*cardsleft==0){
        for(int i=0;i<3;i++){
            onthefield[indexofselected[i]]=empty;
            *numonfield -= 1;
        }
        for(int i=0;i<*numonfield;i++){
            if(cardsAreEqual(onthefield[i],empty)){
                replace[thatone]=i;
                thatone++;
            }
        }
        for(int i=*numonfield;i<temp;i++){
            if(!cardsAreEqual(onthefield[i],empty)){
                replacementCard[thatotherone]=onthefield[i];
                //std::cout<<onthefield[i].multiplicity<<onthefield[i].color<<onthefield[i].fill<<onthefield[i].shape<<std::endl;
                thatotherone++;
            }
        }
        for(int i=0;i<thatone;i++){
            onthefield[replace[i]]=replacementCard[i];
            onthefield[replace[i]].look.setPosition(200*(replace[i]%4),200*(replace[i]/4));
        }
        for(int i=*numonfield;i<temp;i++){
            onthefield[i]=empty;
        }
    }
    if(temp==*numonfield&&*numonfield==12){
    for(int i=0;i<3;i++){
        dealacard(deck,cardsleft,onthefield,numonfield,true,indexofselected[i],randomNumbers);
        onthefield[indexofselected[i]].look.setPosition(200*(indexofselected[i]%4),200*(indexofselected[i]/4));
    }
    std::cout<<"SET!"<<std::endl;
    }
   // if(*cardsleft==0&&temp>*numonfield){
    //    for(int i=0;i<3;i++){
    //        onthefield[indexofselected[i]]=empty;
 //   }
  //  }
    while((!setexists(onthefield,*numonfield,setsOnBoard,d,b,c))&&*cardsleft!=0){
        dealthreecards(deck,cardsleft,onthefield,numonfield,false,0,randomNumbers);
      //  for(int i=0;i<3;i++){
      //          onthefield[*numonfield+i].look.setPosition(200*(indexofselected[i]%4),200*(indexofselected[i]/4));
     //   }
    }
    //if((!setexists(onthefield,*numonfield,setsOnBoard,d,b,c))&&*cardsleft==0) endgame();
    return;
}

void selectacard(card deck[3][3][3][3], card onthefield[21],int *numonfield,card selectarray[3], int *numselected,Vector2i m,int *cardsleft,int *setsOnBoard,int indexofselected[3],int *score,int *d,int *b,int *c,std::vector<int> &randomNumbers){
    int xcard=139;
    int ycard=87;
    int stupidint=0;
    float x=onthefield[stupidint].look.getPosition().x;
    float y=onthefield[stupidint].look.getPosition().y;
    while(m.x<x||m.x>x+xcard||m.y<y||m.y>y+ycard){//continue here
        stupidint++;
        x=onthefield[stupidint].look.getPosition().x;
        y=onthefield[stupidint].look.getPosition().y;
        if(stupidint==*numonfield) return;
    }
        x=onthefield[stupidint].look.getPosition().x;
        y=onthefield[stupidint].look.getPosition().y;
        selectarray[*numselected]=onthefield[stupidint];
        indexofselected[*numselected]=stupidint;
        *numselected += 1;
    if(*numselected==3){
        if(setisfound(selectarray)){
            itisaset(deck,cardsleft,onthefield,numonfield,indexofselected, selectarray,setsOnBoard,score,d,b,c,randomNumbers);
        }
        *numselected=0;
    }
}

void endgame(card deck[3][3][3][3], card onthefield[21], int *setsOnBoard, int *numselected, int *cardsleft, int *numonfield, int *score,int *d,int *b,int *c,std::vector<int> &randomNumbers){
    for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
    for(int k=0;k<3;k++){
    for(int l=0;l<3;l++){
        deck[i][j][k][l].played=false;
    }
    }
    }
    }
    *setsOnBoard=0;
    *numselected=0;
    *cardsleft=81;
    *numonfield=0;
    *score=0;
    for(int i=0;i<81;i++){
        randomNumbers.push_back(i);
    }
    startgame(deck,cardsleft,onthefield,numonfield,false,0,setsOnBoard,d,b,c,randomNumbers);
    setexists(onthefield,*numonfield,setsOnBoard,d,b,c);
}


int main(){
    
    // Create the main window
    RenderWindow window(VideoMode(1920, 1080), "SFML window");

    // Set the Icon
    Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    //Texture texture;
    //if (!texture.loadFromFile(resourcePath() + "cute_image.jpg")) {
    //    return EXIT_FAILURE;
   // }
  //  Sprite sprite(texture);
    
   // Texture testcard;
  //  if (!testcard.loadFromFile(resourcePath() + "empty card.png")) {
  //      return EXIT_FAILURE;
   // }
    
    // Create a graphical text to display
    Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    
    //Text text("SET!", font, 50);
    //text.setFillColor(Color::Magenta);

    // Load a music to play
    Music music;
    if (!music.openFromFile(resourcePath() + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }
    
   
    srand(time(0));
    
    card deck[3][3][3][3];//create the deck array
    std::stringstream fileName;//fill the deck array with all the card details
    for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
    for(int k=0;k<3;k++){
    for(int l=0;l<3;l++){
        deck[i][j][k][l].multiplicity=i;
        deck[i][j][k][l].color=j;
        deck[i][j][k][l].fill=k;
        deck[i][j][k][l].shape=l;
        fileName<<"SET/SET"<<i<<j<<k<<l<<".png";//SET0000.png
        if (!deck[i][j][k][l].style.loadFromFile(resourcePath() +  fileName.str())) {
            return EXIT_FAILURE;
        }
        deck[i][j][k][l].look.setTexture(deck[i][j][k][l].style);
        deck[i][j][k][l].look.setScale(Vector2f(.2,.2));
        //deck[i][j][k][l].cardRectSize.setSize(Vector2f(100,50));
        fileName.str("");
    }
    }
    }
    }

    std::vector<int> randomNumbers;
    for(int i=0;i<81;i++){
        randomNumbers.push_back(i);
    }
    int indexofselected[3];
    int setsOnBoard=0;
    card selectarray[3];
    int numselected=0;
    int cardsleft=81;
    card onthefield[21];
    int numonfield=0;
    int score=0;
    int d;
    int b;
    int c;
    
    Color Purple(177,0,177);
    
    Text scoreD("SETs found = 0", font, 50);
    scoreD.setFillColor(Color::Red);
    
    Text sboard("SETs on board", font, 50);
    sboard.setFillColor(Color::Green);
    
    Text sselect("Number of cards selected ", font, 50);
    sselect.setFillColor(Purple);
    
    Text sfield("Number on field ", font, 50);
    sfield.setFillColor(Color::Red);
    
    Text solutions("Solution indices: ", font, 50);
    solutions.setFillColor(Color::Green);
    
    Text scardsleft("Cards left: ", font, 50);
    scardsleft.setFillColor(Purple);
    
    Text sgameOver("", font, 50);
    sgameOver.setFillColor(Color::Red);

    startgame(deck,&cardsleft,onthefield,&numonfield,false,0,&setsOnBoard,&d,&b,&c,randomNumbers);
    setexists(onthefield,numonfield,&setsOnBoard,&d,&b,&c);
    
    Clock clock;
   
    Time elapsed;
    Text stopwatch("00:00", font, 50);
    stopwatch.setFillColor(Color::Green);
    
    Text highScoreValue;
    Text highScore("High Score: ", font, 50);
    highScore.setFillColor(Color::Red);
    highScore.setStyle(Text::Underlined);
    int highScoreSec=0;
    
    // Play the music
    //music.play();

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            
            // Close window: exit
            if (event.type == Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
            
            Vector2i localPosition;
            if (event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                 localPosition = Mouse::getPosition(window);
                 selectacard(deck,onthefield,&numonfield,selectarray,&numselected,localPosition,&cardsleft,&setsOnBoard,indexofselected,&score,&d,&b,&c,randomNumbers);
             }
            }
            //if (Keyboard::isKeyPressed(Keyboard::Return)){
            //    endgame();//restart game
            //}
            if(cardsleft==0&&setsOnBoard==0){
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                if(highScoreSec==0||highScoreSec>elapsed.asSeconds()){
                    highScoreSec=elapsed.asSeconds();
                    highScoreValue=stopwatch;
                }
                highScoreValue.setPosition(1050,700);
                highScoreValue.setFillColor(Color::Red);
                sgameOver.setString("");
                endgame(deck, onthefield, &setsOnBoard, &numselected, &cardsleft, &numonfield, &score, &d, &b, &c,randomNumbers);
                clock.restart();
                
            }
            }
        }
        
        
        
        if(cardsleft!=0||setsOnBoard!=0){
        elapsed = clock.getElapsedTime();
        }
        else {
            sgameOver.setString("You completed the deck!\nPress Spacebar to start a new game");
        }
        int seconds=elapsed.asSeconds();
        int minutes = seconds/60;
        seconds = seconds%60;
        
        std::stringstream stime;
           
           if(minutes<10 && seconds<10){
                  stime<<"0"<<minutes<<":"<<"0"<<seconds;
           }
           if(minutes<10 && seconds>=10){
                  stime<<"0"<<minutes<<":"<<seconds;
           }
           if(minutes>=10 && seconds<10){
                  stime<<minutes<<":"<<"0"<<seconds;
           }
           if(minutes>=10 && seconds>=10){
                  stime<<minutes<<":"<<seconds;
           }
           stopwatch.setString(stime.str());
           stopwatch.setPosition(750,500);
        
       //std::cout<<fileName.str()<<std::endl;
        
        std::stringstream ss;
        ss<<"Sets found "<<score;
        scoreD.setString(ss.str());
        scoreD.setPosition(750,0);
        
        std::stringstream sob;
        sob<<"SETs on board "<<setsOnBoard;
        sboard.setString(sob.str());
        sboard.setPosition(750,100);
        
        std::stringstream scard;
        scard<<"Number of cards selected "<<numselected;
        sselect.setString(scard.str());
        sselect.setPosition(750,200);
        
        std::stringstream field;
        field<<"Number on field "<<numonfield;
        sfield.setString(field.str());
        sfield.setPosition(750,300);
        
        std::stringstream ssol;
        ssol<<"Solution indices "<<d<<b<<c;
        solutions.setString(ssol.str());
        solutions.setPosition(750,400);
        
        std::stringstream scarde;
        scarde<<"Cards left: "<<cardsleft;
        scardsleft.setString(scarde.str());
        scardsleft.setPosition(750,600);
        
        
        highScore.setPosition(750,700);
        
        
        sgameOver.setPosition(0,500);
        
        // Clear screen
        window.clear();

        // Draw the sprite
        //window.draw(sprite);
        
        
        // Draw the string
        window.draw(sgameOver);
        window.draw(scardsleft);
        window.draw(solutions);
        window.draw(sfield);
        window.draw(scoreD);
        window.draw(sboard);
        window.draw(stopwatch);
        window.draw(sselect);
        window.draw(highScore);
        window.draw(highScoreValue);
        for(int i=0;i<numonfield;i++){
        window.draw(onthefield[i].look);
        }
        //window.delete(stopwatch);
        
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
