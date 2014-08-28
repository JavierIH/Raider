#include "../../libraries/raider/raider.h"
#include <iostream>

#define FW_ANGLE 20
#define FW_DISTANCE 20
#define FL_ANGLE 15
#define FL_DISTANCE 30

using namespace std;
using namespace cv;

int main()
{
    //Setup
    Raider raider;
    int state=0;
    int flag_line=0;

    report(INFO,"Cuenta atrás!");
    report(INFO,"10....");
    usleep(1000000);
    report(INFO,"9...");
    usleep(1000000);
    report(INFO,"8...");
    usleep(1000000);
    report(INFO,"7...");
    usleep(1000000);
    report(INFO,"6...");
    usleep(1000000);
    report(INFO,"5...");
    usleep(1000000);
    report(INFO,"4...");
    usleep(1000000);
    report(INFO,"3...");
    usleep(1000000);
    report(INFO,"2...");
    usleep(1000000);
    report(INFO,"1...");
    usleep(5000000);

    report(INFO,"¡Comienza la prueba!");

    //Algorithm
    while(1){
        // TODO meter inicializaciones, tiempo de espera, boton...
        raider.look();
        usleep(1000000);
        Mat image = raider.getFrame();

        //imshow("image",image);
        cv::Size size(160,120);
        resize(image,image,size);
        switch (state) {

        case 0:{
            //INICIO
            report(STATE, "Estado 0 (Inicio)");
            raider.yes();
            report(MOVE,"S -> Avanti avanti");
            raider.getUp();
            raider.run();
            state=1;
        }break;

        case 1:{
            report(STATE, "Estado 1 (Avance)");
            Vec2i way_result = raider.findWay(image);

            if(abs(way_result[0])>=FW_DISTANCE){
                if(way_result[0]>0){
                    report(MOVE,"E -> Paso lateral a la derecha");
                    raider.getUp();
                    raider.stepR();
                }
                else if(way_result[0]<0){
                    report(MOVE,"Q -> Paso lateral a la izquierda");
                    raider.getUp();
                    raider.stepL();
                }
            }
            else if(abs(way_result[1])>=FW_ANGLE){
                if(way_result[1]<0){
                    report(MOVE,"D -> Girar a la izquierda");
                    raider.getUp();
                    raider.turnL();
                }
                else if(way_result[1]>0){
                    report(MOVE,"A -> Girar a la derecha");
                    raider.getUp();
                    raider.turnR();
                }
            }
            else{
                report(MOVE,"S -> Avanza rapido recto");
                raider.getUp();
                raider.run();
            }
            if(!flag_line) state=2; // -> findLine
            else state=1; // -> findWay
        }break;

        case 2:{
            report(STATE, "Estado 2 (Busqueda de linea)");
            raider.getUp();
            usleep(1000000);
            raider.look();
            usleep(1000000);
            Vec2i line_result = raider.findLine(image);
            report(OK, "LINEA result "+to_string(line_result[0])+"__"+to_string(line_result[1]));

            if (line_result[0]==-1){
                report("No se encuentra la linea");
                state=1; // -> findWay
                break;
            }
            else if(abs(line_result[1])<=FL_ANGLE){
                report(RAIDER,"Voy a cruzar la linea");
                report(MOVE,"W -> Avanzar recto");
                raider.run(); // TODO cerciorarse de que cruza
                raider.getUp();
                raider.run();
                flag_line=true;
                state=3; // -> turnBack
                break;
            }
            else{
                report("Me preparo para cruzar la linea");
                if(line_result[1]<0){
                    report(MOVE,"D -> Girar a la izquierda");
                    raider.turnL();
                }
                else if(line_result[1]>0){
                    report(MOVE,"A -> Girar a la derecha");
                    raider.turnR();
                }
                if(line_result[0]<=FL_DISTANCE){
                    report(RAIDER,"Voy a cruzar la linea "+to_string(FL_ANGLE)+"  _  "+to_string(abs(line_result[0])));
                    report(MOVE,"W -> Avanzar recto");
                    raider.run(); // TODO cerciorarse de que cruza
                    raider.getUp();
                    raider.run();
                    flag_line=true;
                    state=3; // -> turnBack
                    break;
                }
                else{
                    report(RAIDER,"No he cruzado, vuelvo a intentarlo");
                    state=2; // -> findLine
                    break;
                }
            }
        }break;

        case 3:
            report(STATE, "Estado 3 (Vuelta a casa)");
            report("Inicio secuencia para dar la vuelta");
            report(MOVE,"A -> Girar a la derecha X5");
            raider.getUp();
            raider.turnR();
            raider.turnR();
            raider.turnR();
            raider.turnR();
            raider.turnR(); // TODO meter brujula

            state=1;
            break;
        default:
            state=1;
            break;
        } //swicht
        //waitKey(0);

    }//while
}//main
