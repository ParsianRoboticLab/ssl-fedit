#include "Field.h"

#include <QStringList>

CField::CField(void)
{
        fCenter = Vector2D(0.0 , 0.0);
        fOurGoal = Vector2D(- _FIELD_WIDTH/2.0 , 0.0);
        fOppGoal = Vector2D(_FIELD_WIDTH/2.0 , 0.0);
        fOurCornerL = Vector2D(- _FIELD_WIDTH/2.0 , _FIELD_HEIGHT/2.0);
        fOurCornerR = Vector2D(- _FIELD_WIDTH/2.0 , - _FIELD_HEIGHT/2.0);
        fOppCornerL = Vector2D(_FIELD_WIDTH/2.0 , _FIELD_HEIGHT/2.0);
        fOppCornerR = Vector2D(_FIELD_WIDTH/2.0 , - _FIELD_HEIGHT/2.0);
        fOurPenalty = Vector2D(_FIELD_PENALTY - _FIELD_WIDTH/2.0 , 0.0);
        fOppPenalty = Vector2D(_FIELD_WIDTH/2.0 - _FIELD_PENALTY , 0.0);
        fOurGoalL = Vector2D( - _FIELD_WIDTH/2.0, _GOAL_WIDTH / 2.0);
        fOurGoalR = Vector2D( - _FIELD_WIDTH/2.0, -_GOAL_WIDTH / 2.0);
        fOppGoalL = Vector2D(  _FIELD_WIDTH/2.0, _GOAL_WIDTH / 2.0);
        fOppGoalR = Vector2D(  _FIELD_WIDTH/2.0, -_GOAL_WIDTH / 2.0);
        fFieldRect = Rect2D(fOurCornerR+Vector2D(-0.005,-0.005),fOppCornerL+Vector2D(+0.005,+0.005));
		fMarginedFieldRect = Rect2D(fOurCornerR+Vector2D(-0.1,-0.1),fOppCornerL+Vector2D(+0.1,+0.1));
        fOurPenaltyRect  = Rect2D(Vector2D(ourGoal().x,ourGoal().y + 0.975),
                             Vector2D(ourGoal().x + 0.800,ourGoal().y - 0.975));
        fOppPenaltyRect  = Rect2D(Vector2D(oppGoal().x - 0.800,oppGoal().y + 0.975),
                             Vector2D(oppGoal().x ,oppGoal().y - 0.975));

        fOurOneThirdL = Vector2D(-_FIELD_WIDTH / 2.0 + _FIELD_WIDTH / 3.0, _FIELD_HEIGHT / 2.0);
        fOurOneThirdR = Vector2D(-_FIELD_WIDTH / 2.0 + _FIELD_WIDTH / 3.0, -_FIELD_HEIGHT / 2.0);
        fOppOneThirdL = Vector2D(+_FIELD_WIDTH / 2.0 - _FIELD_WIDTH / 3.0, _FIELD_HEIGHT / 2.0);
        fOppOneThirdR = Vector2D(+_FIELD_WIDTH / 2.0 - _FIELD_WIDTH / 3.0, -_FIELD_HEIGHT / 2.0);
}

CField::~CField(void)
{
}

Vector2D CField::center()
{
	return fCenter;
}
Vector2D CField::ourGoal()
{
	return fOurGoal;
}
Vector2D CField::oppGoal()
{
	return fOppGoal;
}
Vector2D CField::ourCornerL()
{
	return fOurCornerL;
}
Vector2D CField::ourCornerR()
{
	return fOurCornerR;
}
Vector2D CField::oppCornerL()
{
	return fOppCornerL;
}
Vector2D CField::oppCornerR()
{
	return fOppCornerR;
}
Vector2D CField::ourPenalty()
{
	return fOurPenalty;
}
Vector2D CField::oppPenalty()
{
	return fOppPenalty;
}

Vector2D CField::ourGoalL() {
	return fOurGoalL;
}

Vector2D CField::ourGoalR() {
	return fOurGoalR;
}

Vector2D CField::oppGoalL() {
	return fOppGoalL;
}

Vector2D CField::oppGoalR() {
	return fOppGoalR;
}

Rect2D   CField::fieldRect() {
        return fFieldRect;
}

Rect2D   CField::marginedField(){
		return fMarginedFieldRect;
}

bool     CField::isInField(Vector2D point){
    return fFieldRect.contains(point);
}

Rect2D   CField::ourPenaltyRect()
{
    return fOurPenaltyRect;
}

Rect2D   CField::oppPenaltyRect()
{
    return fOppPenaltyRect;
}

Vector2D CField::ourOneThirdL()
{
    return fOurOneThirdL;
}

Vector2D CField::ourOneThirdR()
{
    return fOurOneThirdR;
}

Vector2D CField::oppOneThirdL()
{
    return fOppOneThirdL;
}

Vector2D CField::oppOneThirdR()
{
    return fOppOneThirdR;
}

bool CField::isInPenaltyArea(Vector2D point, bool oppPenalty)
{
    //TODO: consider circle like shape of penalty area
    if (oppPenalty) {return fOppPenaltyRect.contains(point);}
    {return fOurPenaltyRect.contains(point);}
}

QList<Vector2D> CField::ourPAreaIntersect(Line2D line)
{

}

QList<Vector2D> CField::ourBigPAreaIntersect(Line2D line, float scale, float bias)
{

}


QList<Vector2D> CField::ourPAreaIntersect(Segment2D segment)
{

}

QList<Vector2D> CField::ourPAreaIntersect(Circle2D circle)
{

}

bool CField::isInOurPArea(Vector2D point)
{

}



QList<Vector2D> CField::oppPAreaIntersect(Segment2D segment)
{

}

QList<Vector2D> CField::oppPAreaIntersect(Circle2D circle)
{

}

bool CField::isInOppPArea(Vector2D point)
{

}

Rect2D   CField::getRegion(Region region, double k)
{
    Rect2D tmp;
    k *= 0.5;
    double d = 0.33;
    switch (region)            
    {
        case OppCornerTop   : return Rect2D(Vector2D( _FIELD_WIDTH*k, _FIELD_HEIGHT*0.5), Vector2D( _FIELD_WIDTH*0.5, _GOAL_WIDTH*0.5 + d));
        case OppCornerBottom: return Rect2D(Vector2D( _FIELD_WIDTH*k,-_FIELD_HEIGHT*0.5), Vector2D( _FIELD_WIDTH*0.5,-_GOAL_WIDTH*0.5 - d));
        case OurCornerBottom: return Rect2D(Vector2D(-_FIELD_WIDTH*k,-_FIELD_HEIGHT*0.5), Vector2D(-_FIELD_WIDTH*0.5,-_GOAL_WIDTH*0.5 - d));
        case OurCornerTop   : return Rect2D(Vector2D(-_FIELD_WIDTH*k, _FIELD_HEIGHT*0.5), Vector2D(-_FIELD_WIDTH*0.5, _GOAL_WIDTH*0.5 + d));
        case MidFieldTop    : return Rect2D(Vector2D(-_FIELD_WIDTH*k, _FIELD_HEIGHT*0.5), Vector2D(+_FIELD_WIDTH*k, _GOAL_WIDTH*0.5 + d));
        case MidFieldBottom : return Rect2D(Vector2D(+_FIELD_WIDTH*k,-_FIELD_HEIGHT*0.5), Vector2D(-_FIELD_WIDTH*k,-_GOAL_WIDTH*0.5 - d));
        case FrontGoalOpp   : return Rect2D(Vector2D(+_FIELD_WIDTH*k,+_GOAL_WIDTH*0.5 + d), Vector2D(_FIELD_WIDTH*0.5-0.5,-_GOAL_WIDTH*0.5 - d));
        case FrontGoalOur   : return Rect2D(Vector2D(-_FIELD_WIDTH*0.5+1.5, +_GOAL_WIDTH*0.5 + d), Vector2D(-_FIELD_WIDTH*0.5+2.5,-_GOAL_WIDTH*0.5 - d));
        case Center         : return Rect2D(Vector2D(-_FIELD_WIDTH*k, +_GOAL_WIDTH*0.5 + d), Vector2D(+_FIELD_WIDTH*k,-_GOAL_WIDTH*0.5 - d));
        case BigCenter      : return Rect2D(Vector2D(-1.5, -1.5), Vector2D(1.5, 1.5));
        case OurPenalty     : return fOurPenaltyRect;
        case OppPenalty     : return fOppPenaltyRect;
        case OurField       : return Rect2D(Vector2D(-_FIELD_WIDTH*0.5, _FIELD_HEIGHT*0.5), Vector2D(0.0,-_FIELD_HEIGHT*0.5));
        case OppField       : return Rect2D(Vector2D(0.0, _FIELD_HEIGHT*0.5), Vector2D(_FIELD_WIDTH*0.5,-_FIELD_HEIGHT*0.5));
        case OppCornerBottomLeft : tmp = getRegion(OppCornerBottom);tmp.setLength(tmp.size().length()*0.5);return tmp;
        case OppCornerTopLeft    : tmp = getRegion(OppCornerTop);tmp.setLength(tmp.size().length()*0.5);return tmp;
        case OppCornerBottomRight: tmp = getRegion(OppCornerBottom);tmp.setLength(tmp.size().length()*0.5);tmp.assign(tmp.topRight(), tmp.size());return tmp;
        case OppCornerTopRight   : tmp = getRegion(OppCornerTop);tmp.setLength(tmp.size().length()*0.5);tmp.assign(tmp.topRight(), tmp.size());return tmp;
        case OppCornerPointTop    : return Rect2D(Vector2D(oppCornerL().x-0.45, oppCornerL().y - 0.45) , oppCornerL());
        case OppCornerPointBottom: return Rect2D(Vector2D(oppCornerR().x-0.45, oppCornerR().y + 0.45) , oppCornerR());
        case OurMidFieldTop      : tmp = getRegion(MidFieldTop);tmp.setLength(tmp.size().length()*0.5);return tmp;
        case OurMidFieldBottom   : tmp = getRegion(MidFieldBottom);tmp.setLength(tmp.size().length()*0.5);return tmp;
        case OppMidFieldTop      : tmp = getRegion(MidFieldTop);tmp.setLength(tmp.size().length()*0.5);tmp.assign(tmp.topRight(), tmp.size());return tmp;
        case OppMidFieldBottom   : tmp = getRegion(MidFieldBottom);tmp.setLength(tmp.size().length()*0.5);tmp.assign(tmp.topRight(), tmp.size());return tmp;
        case OppFieldBottom      : return Rect2D(Vector2D(0.0, 0.0), Vector2D(_FIELD_WIDTH*0.5,-_FIELD_HEIGHT*0.5));
        case OppFieldTop         : return Rect2D(Vector2D(0.0, _FIELD_HEIGHT*0.5), Vector2D(_FIELD_WIDTH*0.5,0.0));
        case OurFieldBottom      : return Rect2D(Vector2D(-_FIELD_WIDTH*0.5, 0.0), Vector2D(0.0,-_FIELD_HEIGHT*0.5));
        case OurFieldTop         : return Rect2D(Vector2D(-_FIELD_WIDTH*0.5, _FIELD_HEIGHT*0.5), Vector2D(0.0,0.0));
        case OurMidFieldTopMost    : tmp = getRegion(MidFieldTop);tmp.setLength(tmp.size().length()*0.5);tmp.setWidth(tmp.size().width()*0.5);tmp.assign(tmp.topLeft(), tmp.size());tmp.setTopLeft(tmp.topLeft()+Vector2D(-0.3,0.0));return tmp;
        case OurMidFieldBottomMost : tmp = getRegion(MidFieldBottom);tmp.setLength(tmp.size().length()*0.5);tmp.setWidth(tmp.size().width()*0.5);tmp.assign(tmp.bottomLeft(), tmp.size());tmp.setTopLeft(tmp.topLeft()+Vector2D(-0.3,0.0));return tmp;
        case Field1stQuarter     : return Rect2D(Vector2D(-_FIELD_WIDTH*0.5, _FIELD_HEIGHT*0.5), Vector2D(-_FIELD_WIDTH*0.25,-_FIELD_HEIGHT*0.5));
        case Field2ndQuarter     : return Rect2D(Vector2D(-_FIELD_WIDTH*0.25, _FIELD_HEIGHT*0.5), Vector2D(0.0,-_FIELD_HEIGHT*0.5));
        case Field3rdQuarter     : return Rect2D(Vector2D(0.0, _FIELD_HEIGHT*0.5), Vector2D(_FIELD_WIDTH*0.25,-_FIELD_HEIGHT*0.5));
        case Field4thQuarter     : return Rect2D(Vector2D(_FIELD_WIDTH*0.25, _FIELD_HEIGHT*0.5), Vector2D(_FIELD_WIDTH*0.5,-_FIELD_HEIGHT*0.5));
        case OurMidFieldTopWing  : return Rect2D(Vector2D(-0.2, _FIELD_HEIGHT*0.5), Vector2D(-0.1,1.4));
        case OurMidFieldBottomWing  : return Rect2D(Vector2D(-0.2, -_FIELD_HEIGHT*0.5), Vector2D(-0.1,-1.4));
        case OurCornerBottomLeft : tmp = getRegion(OurCornerBottom);tmp.setLength(tmp.size().length()*0.5);return tmp;
        case OurCornerTopLeft    : tmp = getRegion(OurCornerTop);tmp.setLength(tmp.size().length()*0.5);return tmp;
        case OurCornerBottomRight: tmp = getRegion(OurCornerBottom);tmp.setLength(tmp.size().length()*0.5);tmp.assign(tmp.topRight(), tmp.size());return tmp;
        case OurCornerTopRight   : tmp = getRegion(OurCornerTop);tmp.setLength(tmp.size().length()*0.5);tmp.assign(tmp.topRight(), tmp.size());return tmp;
		case OurCornerPointTop    : return Rect2D(ourCornerL() , Vector2D(ourCornerL().x+0.45, ourCornerL().y - 0.45));
		case OurCornerPointBottom: return Rect2D(Vector2D(ourCornerR().x, ourCornerR().y + 0.45) , Vector2D(ourCornerR().x+0.45, ourCornerR().y));


            // For Mani
        case OurDeffenseOneThird : return Rect2D(Vector2D( (-_FIELD_WIDTH/2.0)-0.2 , (-_FIELD_HEIGHT/2.0)-0.2), Vector2D( (-_FIELD_WIDTH/2.0)+(_FIELD_WIDTH/3.0) , (_FIELD_HEIGHT/2.0)+0.2));
        case OurAttackOneThird   : return Rect2D(Vector2D( (_FIELD_WIDTH/2.0)+(-_FIELD_WIDTH/3.0) , (-_FIELD_HEIGHT/2.0)-0.2), Vector2D( (_FIELD_WIDTH/2.0)+0.2 , (_FIELD_HEIGHT/2.0)+0.2));
        case OurMidOneThird      : return Rect2D(Vector2D( (-_FIELD_WIDTH/2.0)+(_FIELD_WIDTH/3.0) , (-_FIELD_HEIGHT/2.0)-0.2) , Vector2D( (_FIELD_WIDTH/2.0)+(-_FIELD_WIDTH/3.0) , (_FIELD_HEIGHT/2.0)+0.2));
        case OppMidField         : return Rect2D(Vector2D(0, -_GOAL_WIDTH*0.5 - d), Vector2D(+_FIELD_WIDTH*k, _GOAL_WIDTH*0.5 + d));
        case OppCornerLineTop    : return Rect2D(Vector2D(_FIELD_WIDTH/2.0 - 0.55, 1.2), Vector2D(_FIELD_WIDTH/2.0 - 0.4, 0.95));
        case OppCornerLineBottom : return Rect2D(Vector2D(_FIELD_WIDTH/2.0 - 0.55, -0.95), Vector2D(_FIELD_WIDTH/2.0 - 0.4, -1.2));
        case FieldGrid1Top       : return Rect2D(Vector2D( -1.5, _FIELD_HEIGHT/2.0), Vector2D( -0.75, 0.5));
        case FieldGrid1Center    : return Rect2D(Vector2D( -1.5, 0.5), Vector2D( -0.75, -0.5));
        case FieldGrid1Bottom    : return Rect2D(Vector2D( -1.5, -0.5), Vector2D( -0.75, -_FIELD_HEIGHT/2.0));
        case FieldGrid2Top       : return Rect2D(Vector2D( -0.75, _FIELD_HEIGHT/2.0), Vector2D( 0.0, 0.5));
        case FieldGrid2Center    : return Rect2D(Vector2D( -0.75, 0.5), Vector2D( 0.0, -0.5));
        case FieldGrid2Bottom    : return Rect2D(Vector2D( -0.75, -0.5), Vector2D( 0.0, -_FIELD_HEIGHT/2.0));
        case FieldGrid3Top       : return Rect2D(Vector2D( 0.0, _FIELD_HEIGHT/2.0), Vector2D( 0.75, 0.5));
        case FieldGrid3Center    : return Rect2D(Vector2D( 0.0, 0.5), Vector2D( 0.75, -0.5));
        case FieldGrid3Bottom    : return Rect2D(Vector2D( 0.0, -0.5), Vector2D( 0.75, -_FIELD_HEIGHT/2.0));
        case FieldGrid4Top       : return Rect2D(Vector2D( 0.75, _FIELD_HEIGHT/2.0), Vector2D( 1.5, 0.5));
        case FieldGrid4Center    : return Rect2D(Vector2D( 0.75, 0.5), Vector2D( 1.5, -0.5));
        case FieldGrid4Bottom    : return Rect2D(Vector2D( 0.75, -0.5), Vector2D( 1.5, -_FIELD_HEIGHT/2.0));
        case FieldGrid5Top       : return Rect2D(Vector2D( 1.5, _FIELD_HEIGHT/2.0), Vector2D( 2.25, 0.5));
        case FieldGrid5Center    : return Rect2D(Vector2D( 1.5, 0.5), Vector2D( 2.25, -0.5));
        case FieldGrid5Bottom    : return Rect2D(Vector2D( 1.5, -0.5), Vector2D( 2.25, -_FIELD_HEIGHT/2.0));
        case TheirPenaltyTop     : return Rect2D(Vector2D(2.38349,1.33186),Vector2D(2.71047,1.06512));
        case TheirPenaltyBottom  : return Rect2D(Vector2D(2.38349,-1.33186),Vector2D(2.71047,-1.06512));
		case AttackRecCornerTopP1 : return Rect2D(Vector2D(1.3,-0.2),Vector2D(2.9,-1.7));
        case AttackRecCornerTopP2 : return Rect2D(Vector2D(0,1.8),Vector2D(2,0));

		case AttackRecCornerBottomP1 : return Rect2D(Vector2D(1.3,1.9),Vector2D(2.7,0.2));
        case AttackRecCornerBottomp2 : return Rect2D(Vector2D(0,0),Vector2D(2,-1.8));
        case AttackRecMidTopP2  : return Rect2D(Vector2D(0,-0.3),Vector2D(1.5,-1.8));


        case AttackRecMidBottomp2   : return Rect2D(Vector2D(0,1.8),Vector2D(1.5,0.3));
        case TC2012Top1 : return Rect2D(Vector2D(-3 , 2),Vector2D(-1.8 , 0.8));
        case TC2012Top2 : return Rect2D(Vector2D(-1.8 , 2),Vector2D(-0.6 , 0.8));
        case TC2012Top3 : return Rect2D(Vector2D(-0.6 , 2),Vector2D(0.6 , 0.8));
        case TC2012Top4 : return Rect2D(Vector2D(0.6 , 2),Vector2D(2.5 , 0.8));
        case TC2012Bottom1 : return Rect2D(Vector2D(-3 , -0.8),Vector2D(-1.8 , -2));
        case TC2012Bottom2 : return Rect2D(Vector2D(-1.8 , -0.8),Vector2D(-0.6 , -2));
        case TC2012Bottom3 : return Rect2D(Vector2D(-0.6 , -0.8),Vector2D(0.6 , -2));
        case TC2012Bottom4 : return Rect2D(Vector2D(0.6 , -0.8),Vector2D(2.5 , -2));
		case TC2012Rect1 : return Rect2D(Vector2D(0,2) , Vector2D(1.5,0));
		case TC2012Rect2 : return Rect2D(Vector2D(3,2) , Vector2D(1.5,0));
		case TC2012Rect3 : return Rect2D(Vector2D(1.5,0) , Vector2D(0,-2));
		case TC2012Rect4 : return Rect2D(Vector2D(1.5,0) , Vector2D(3,-2));
	}
    return Rect2D(Vector2D::INVALIDATED,Vector2D::INVALIDATED);
}

Rect2D  CField::getCircleRegion(int n, int i)
{	

}

Rect2D  CField::getRegion(QString name, double k)
{
    Region r=OppField;
    if (name=="ourcornertop") r=CField::OurCornerTop;
    else if (name=="ourcornerbottom") r=CField::OurCornerBottom;
    else if (name=="oppcornertop") r=CField::OppCornerTop;
    else if (name=="oppcornerbottom") r=CField::OppCornerBottom;
    else if (name=="midfieldtop") r=CField::MidFieldTop;
    else if (name=="midfieldbottom") r=CField::MidFieldBottom;
    else if (name=="frontgoalopp") r=CField::FrontGoalOpp;
    else if (name=="frontgoalour") r=CField::FrontGoalOur;
    else if (name=="center") r=CField::Center;
    else if (name=="ourpenalty") r=CField::OurPenalty;
    else if (name=="opppenalty") r=CField::OppPenalty;
    else if (name=="ourfield") r=CField::OurField;
    else if (name=="oppfield") r=CField::OppField;
    else if (name=="oppcornertopright") r=CField::OppCornerTopRight;
    else if (name=="oppcornertopleft") r=CField::OppCornerTopLeft;
    else if (name=="oppcornerbottomright") r=CField::OppCornerBottomRight;
    else if (name=="oppcornerbottomleft")  r=CField::OppCornerBottomLeft;
    else if (name=="ourcornertopright") r=CField::OurCornerTopRight;
    else if (name=="ourcornertopleft") r=CField::OurCornerTopLeft;
    else if (name=="ourcornerbottomright") r=CField::OurCornerBottomRight;
    else if (name=="ourcornerbottomleft")  r=CField::OurCornerBottomLeft;
    else if (name=="ourmidfieldtop") r=CField::OurMidFieldTop;
    else if (name=="ourmidfieldbottom") r=CField::OurMidFieldBottom ;
    else if (name=="oppmidfieldtop") r=CField::OppMidFieldTop;
    else if (name=="oppmidfieldbottom") r=CField::OppMidFieldBottom;
    else if (name=="oppfieldtop") r=CField::OppFieldTop;
    else if (name=="oppfieldbottom") r=CField::OppFieldBottom;
    else if (name=="ourfieldtop") r=CField::OurFieldTop;
    else if (name=="ourfieldbottom") r=CField::OurFieldBottom;
    else if (name=="ourmidfieldtopmost") r=CField::OurMidFieldTopMost;
    else if (name=="ourmidfieldbottommost") r=CField::OurMidFieldBottomMost;
    else if (name=="field1stquarter") r=CField::Field1stQuarter;
    else if (name=="field2ndquarter") r=CField::Field2ndQuarter;
    else if (name=="field3rdquarter") r=CField::Field3rdQuarter;
    else if (name=="field4thquarter") r=CField::Field4thQuarter;
    else if (name=="ourmidfieldtopwing") r=CField::OurMidFieldTopWing;
    else if (name=="ourmidfieldbottomwing") r=CField::OurMidFieldBottomWing;
    else if (name=="ourattackonethird") r=CField::OurAttackOneThird;
    else if (name=="ourdeffenseonethird") r=CField::OurDeffenseOneThird;
    else if (name=="ourmidonethird") r=CField::OurMidOneThird;
    else if (name=="oppmidfield") r=CField:: OppMidField;
    else if (name=="oppcornerlinetop") r=CField:: OppCornerLineTop;
    else if (name=="oppcornerlinebottom") r=CField:: OppCornerLineBottom;
    else if (name=="fieldgrid1top") r=CField:: FieldGrid1Top;
    else if (name=="fieldgrid1center") r=CField:: FieldGrid1Center;
    else if (name=="fieldgrid1bottom") r=CField:: FieldGrid1Bottom;
    else if (name=="fieldgrid2top") r=CField:: FieldGrid2Top;
    else if (name=="fieldgrid2center") r=CField:: FieldGrid2Center;
    else if (name=="fieldgrid2bottom") r=CField:: FieldGrid2Bottom;
    else if (name=="fieldgrid3top") r=CField:: FieldGrid3Top;
    else if (name=="fieldgrid3center") r=CField:: FieldGrid3Center;
    else if (name=="fieldgrid3bottom") r=CField:: FieldGrid3Bottom;
    else if (name=="fieldgrid4top") r=CField:: FieldGrid4Top;
    else if (name=="fieldgrid4center") r=CField:: FieldGrid4Center;
    else if (name=="fieldgrid4bottom") r=CField:: FieldGrid4Bottom;
    else if (name=="fieldgrid5top") r=CField:: FieldGrid5Top;
    else if (name=="fieldgrid5center") r=CField:: FieldGrid5Center;
    else if (name=="fieldgrid5bottom") r=CField:: FieldGrid5Bottom;
    else if (name=="oppcornerpointtop") r=CField:: OppCornerPointTop;
    else if (name=="oppcornerpointbottom") r=CField:: OppCornerPointBottom;
    else if (name=="theirpenaltytop") r=CField:: TheirPenaltyTop;
    else if (name=="theirpenaltybottom") r=CField:: TheirPenaltyBottom;
    else if (name=="attreccornertopb") r=CField::AttackRecCornerTopB;
    else if (name=="attreccornertopp1") r=CField::AttackRecCornerTopP1;
    else if (name=="attreccornertopp2") r=CField::AttackRecCornerTopP2;
    else if (name=="attreccornerbottomb") r=CField::AttackRecCornerBottomB;
    else if (name=="attreccornerbottomp1") r=CField::AttackRecCornerBottomP1;
    else if (name=="attreccornerbottomp2") r=CField::AttackRecCornerBottomp2;
    else if (name=="attrecmidtopb") r=CField::AttackRecMidTopB;
    else if (name=="attrecmidtopp1") r=CField::AttackRecMidTopP1;
    else if (name=="attrecmidtopp2") r=CField::AttackRecMidTopP2;
    else if (name=="attrecmidbottomb") r=CField::AttackRecMidBottomB;
    else if (name=="attrecmidbottomp1") r=CField::AttackRecMidBottomP1;
    else if (name=="attrecmidbottomp2") r=CField::AttackRecMidBottomp2;
	else if (name=="tc2012top1") r=CField::TC2012Top1;
	else if (name=="tc2012top2") r=CField::TC2012Top2;
	else if (name=="tc2012top3") r=CField::TC2012Top3;
	else if (name=="tc2012top4") r=CField::TC2012Top4;
	else if (name=="tc2012bottom1") r=CField::TC2012Bottom1;
	else if (name=="tc2012bottom2") r=CField::TC2012Bottom2;
	else if (name=="tc2012bottom3") r=CField::TC2012Bottom3;
	else if (name=="tc2012bottom4") r=CField::TC2012Bottom4;
	else if (name=="tc2012rect1") r==CField::TC2012Rect1;
	else if (name=="tc2012rect2") r==CField::TC2012Rect2;
	else if (name=="tc2012rect3") r==CField::TC2012Rect3;
	else if (name=="tc2012rect4") r==CField::TC2012Rect4;
	else if (name=="ourcornerpointtop") r=CField:: OurCornerPointTop;
	else if (name=="ourcornerpointbottom") r=CField:: OurCornerPointBottom;
    else if (name=="bigcenter") r = CField::BigCenter;
	else if (name.startsWith("circle"))
    {
        QStringList q = name.right(name.length()-6).split("_");
        if (q.length() == 2)
        {
            bool ok;
            int n = q.first().toInt(&ok);
            if (ok)
            {
                int i = q.last().toInt(&ok);
                if (ok)
                {
                    return getCircleRegion(n, i);
                }
            }
        }
    }
    return getRegion(r, k);
}


Vector2D CField::ourPAreaPerpendicularVector(double angle, Vector2D& intersectpoint)
{

}

double CField::ourPAreaPos(double angle)
{

}
