// Swapnil Paratey - CSCI 5229 Computer Graphics - Summer 2015
// Final Project

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "CSCIx229.h"
#include "terrain.cpp"

using namespace std;

#define LEN 8192    // Maximum length of text string

int axes = 0;       // Display axes
int fov = 70;       // Field of view (for perspective)
double asp = 1;     // Aspect ratio
double dim = 100.0; // Size of the orthogonal box
// float x,z
int dino, house, tree, humvee, heli;

// Light values
int Th = 0, Ph = 0; // Sun directional angles
float sco = 180;    //  Spot cutoff angle
float sco1 = 11;    // For the spotlight
float Exp = 0;      //  Spot exponent
int at0 = 100;      //  Constant  attenuation %
int at1 = 0;        //  Linear    attenuation %
int at2 = 0;        //  Quadratic attenuation %
int side = 0;       // Keep two-sided off
int dist = 200;     // Light 
int smooth = 1;     // Smooth/Flat shading
int local = 0;      // Local Viewer Model
int emission = 100; // Emission intensity (%)
int ambient = 0;    // Ambient intensity (%)
int diffuse = 100;  // Diffuse intensity (%)
int specular = 100; // Specular intensity (%)
int shininess = 0;  // Shininess (power of two)
float shinyvec[]={1}; // Shininess (value)
float X = 0;        // Light X position
float Y = 0;        // Light Y position
float Z = 1;        // Light Z position
int zh  = 90;       // Light azimuth
float ylight = 5.5; // Elevation of light
int light = 1, spotlight = 0;   // Toggle flags for global light and heli-light
int move = 1;       // Toggle lighting
float angle = 0.0f; // Angle of rotation for strafe rotate
float D = 5.0;      // Radius of the LookAt position compared to Eye position
float strafe_step_factor = 0.1; // How long should the strafe jump
int num = 1;        // Number of quads
float Sx=0.0f, Sy=0.0f, Sz=0.0f; // Spot light pointing
// Eye-pointing variables
double Ex = 49.0, Ey = 11.0, Ez = -39.0, lx = -4.0, lz = 3.0;

unsigned int texture[10]; // Textures

// Draw a road based on the co-ordinates
static void drawRoad(double x1, double x2, double z1, double z2, double height)
{   
    float i;
    
    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    //  Texture Enabling
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    // Draw the road.
    glNormal3f(0, -1, -1);
    for(i=x1;i<=x2;i++)
    {
        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glTexCoord2f(0, 0); glVertex3f(i, -height, z1);
        glTexCoord2f(1, 0); glVertex3f(i, -height, z2);
        glTexCoord2f(1, 1); glVertex3f(i+1, -height, z2);
        glTexCoord2f(0, 1); glVertex3f(i+1, -height, z1);
        glEnd();
    }     
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
}

//Draw Dinosaur
static void drawDinosaur(double x, double y, double z, double dx, double dy, double dz, double th)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(th, 0, 1, 0);
    glScaled(dx,dy,dz);
    glCallList(dino);
    glPopMatrix();
    
}

//Draw House
static void drawHouse(double x, double y, double z, double dx, double dy, double dz, double th)
{    
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(th, 0, 1, 0);
    glScaled(dx,dy,dz);
    glCallList(house);
    glPopMatrix();
}

//Draw Humvee
static void drawHumvee(double x, double y, double z, double dx, double dy, double dz, double th, double th2)
{    
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(th, 1, 0, 0);
    glRotatef(th2, 0, 1, 0);
    glScaled(dx,dy,dz);
    glCallList(humvee);
    glPopMatrix();
}

//Draw Helicopter
static void drawHeli(double x, double y, double z, double dx, double dy, double dz, double th)
{    
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(th, 0, 1, 0);
    glScaled(dx,dy,dz);
    glCallList(heli);
    glPopMatrix();
}

//Draw tree
static void drawTree(double x, double y, double z, double dx, double dy, double dz, double th)
{    
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(th, 1, 0, 0);
    glScaled(dx,dy,dz);
    glCallList(tree);
    glPopMatrix();
}

// Draw the building with location x,y,z; scaling dx, dy, dz; rotation th; width height and length of the building as w,h,l and texture int
static void building(double x, double y, double z, double dx, double dy, double dz, double th, double w, double h, double l, unsigned int texturez)
{
    double i, j;
    //  Save transformation
    glPushMatrix();
    
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    // Textures enable
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D, texturez);
    for (j = 0; j <= h; j++)
    {
        // Back -X
        glNormal3d(0,0,-1);
        for(i = -w/2; i < w/2; i++)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(i, j, -l/2);
            glTexCoord2f(1, 0); glVertex3f(i, j+1, -l/2);
            glTexCoord2f(1, 1); glVertex3f(i+1, j+1, -l/2);
            glTexCoord2f(0, 1); glVertex3f(i+1, j, -l/2); 
            glEnd();           
        }
        
        // Front +X
        glNormal3d(0,0,1);
        for(i = -w/2; i < w/2; i++)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(i, j, l/2);
            glTexCoord2f(1, 0); glVertex3f(i, j+1, l/2);
            glTexCoord2f(1, 1); glVertex3f(i+1, j+1, l/2);
            glTexCoord2f(0, 1); glVertex3f(i+1, j, l/2); 
            glEnd();           
        }
        
        // Left -Z
        glNormal3d(1,0,0);
        for(i = -l/2; i < l/2; i++)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(w/2, j, i);
            glTexCoord2f(1, 0); glVertex3f(w/2, j+1, i);
            glTexCoord2f(1, 1); glVertex3f(w/2, j+1, i+1);
            glTexCoord2f(0, 1); glVertex3f(w/2, j, i+1); 
            glEnd();           
        }        
        
        // Right  +Z
        glNormal3d(-1,0,0);
        for(i = -l/2; i < l/2; i++)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(-w/2, j, i);
            glTexCoord2f(1, 0); glVertex3f(-w/2, j+1, i);
            glTexCoord2f(1, 1); glVertex3f(-w/2, j+1, i+1);
            glTexCoord2f(0, 1); glVertex3f(-w/2, j, i+1); 
            glEnd();           
        }            
    }
    
    // Just for top of the building
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(w/2, h+1, -l/2);
    glTexCoord2f(1, 0); glVertex3f(-w/2, h+1, -l/2);
    glTexCoord2f(1, 1); glVertex3f(-w/2, h+1, l/2);
    glTexCoord2f(0, 1); glVertex3f(w/2, h+1, l/2); 
    glEnd();           
    
    
    glDisable(GL_TEXTURE_2D); 
    
    
    
    
    
    glPopMatrix();    
}

// SkyCube with texture
static void SkyCube(double D)
{
    
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    
    //  Sides
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.00,0.36); glVertex3f(-D,-D/2,-D);
    glTexCoord2f(0.25,0.36); glVertex3f(+D,-D/2,-D);
    glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.00,0.66); glVertex3f(-D,+D,-D);
    
    glTexCoord2f(0.25,0.36); glVertex3f(+D,-D/2,-D);
    glTexCoord2f(0.50,0.36); glVertex3f(+D,-D/2,+D);
    glTexCoord2f(0.50,0.66); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
    
    glTexCoord2f(0.50,0.36); glVertex3f(+D,-D/2,+D);
    glTexCoord2f(0.75,0.36); glVertex3f(-D,-D/2,+D);
    glTexCoord2f(0.75,0.66); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.50,0.66); glVertex3f(+D,+D,+D);
   
    glTexCoord2f(0.75,0.36); glVertex3f(-D,-D/2,+D);
    glTexCoord2f(1.00,0.36); glVertex3f(-D,-D/2,-D);
    glTexCoord2f(1.00,0.66); glVertex3f(-D,+D,-D);
    glTexCoord2f(0.75,0.66); glVertex3f(-D,+D,+D);
    glEnd();
    
    //  Top and bottom
    glBegin(GL_QUADS);
    glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.5,0.66); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.25,1); glVertex3f(-D,+D,-D);

    // Keep bottom open for debugging     
    glTexCoord2f(0.25,0.0); glVertex3f(+D,-D/2,-D);
    glTexCoord2f(0.5,0.0); glVertex3f(+D,-D/2,+D);
    glTexCoord2f(0.5,0.36); glVertex3f(-D,-D/2,+D);
    glTexCoord2f(0.25,0.36); glVertex3f(-D,-D/2,-D);    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

// The sun
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Yellow ball
   glColor3f(1,1,0);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

// OpenGL (GLUT) calls this routine to display the scene
void display()
{
    const double len=50.0;  //  Length of axes

    float Position[] = {(float)ylight,(float)(X+dist*Cos(Th)),(float)(Y+dist*Sin(Th)), 1.0}; // Position of the sun light source
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    // Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    // Not using CULL_FACE
    // glEnable(GL_CULL_FACE);
   	glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
    
    // Translate intensity to colors
    float Ambient[]   = {(float)0.01*ambient ,(float)0.01*ambient ,(float)0.01*ambient ,1.0};
    float Diffuse[]   = {(float)0.01*diffuse ,(float)0.01*diffuse ,(float)0.01*diffuse ,1.0};
    float Specular[]  = {(float)0.01*specular,(float)0.01*specular,(float)0.01*specular,1.0};
    float yellow[] = {1.0,0.6,0.0,1.0}; // More like orange
    
    // Strafe according to lookat position
    gluLookAt(Ex, Ey, Ez, Ex+lx, Ey, Ez+lz, 0, 1.0f, 0);
    
    // Textures should MODULATE based on lighting
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    // Flat or smooth shading
    glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
    
    // Lighting toggle
    if (light)
    {
      // Always point at the origin, no need to calculate
      float Direction[] = {0,0,0,1}; 
      //  Draw light position as ball (still no lighting here)
      ball(Position[0],Position[1],Position[2] , 10);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  Two sided mode
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Set specular colors
      glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
      glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      //  Set spotlight parameters
      glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
      glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
      glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);
      //  Set attenuation
      glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,at2/100.0);
    }
    else
      glDisable(GL_LIGHTING);
    
    // Draw the background
    SkyCube(100);
    
    // Set helicopter spotlight
    if(spotlight == 1)
    {
    
        float SpotLightDirection[] = {Sx, -100, Sz, 0.0};
        float SpotLightPosition[] = {(float)(Ex+3*lx),(float)(Ey-1), float(Ez+3*lz), 1.0};
        // drawSphere(Ex+3*lx, Ey-1, Ez+3*lz, 1.0);
        drawHeli(Ex+2*lx, Ey-2, Ez+2*lz, 0.01, 0.01, 0.01, angle*60);
        glEnable(GL_LIGHT1);
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  Two sided mode
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Set specular colors
        glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
        glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);    
        glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT1,GL_POSITION,SpotLightPosition);
          //  Set spotlight parameters
        glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,SpotLightDirection);
        glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,sco1);
        glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,Exp);
          //  Set attenuation
        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,at0/100.0);
        glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,at1/100.0);
        glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,at2/100.0);
    }
    else
        glDisable(GL_LIGHT1); // Disable helicopter lighting on toggle
    
    // Start drawing things on the scene  
    building(-50, 0, 0, 1.0, 1.0, 1.0, 0, 10, 20, 10, texture[4]);
    building(-50, 0, -12.5, 1.0, 1.0, 1.0, 0, 10, 30, 10, texture[4]);
    building(-50, 0, -45, 1.0, 1.0, 1.0, 0, 5, 30, 10, texture[5]);
    building(-44, 0, -45, 1.0, 1.0, 1.0, 0, 5, 5, 10, texture[6]); 
    building(-35, 0, -45, 1.0, 1.0, 1.0, 0, 10, 15, 10, texture[5]); 
    building(5, 0, -45, 1.0, 1.0, 1.0, 0, 10, 25, 10, texture[8]);
    building(5, 0, -45, 1.0, 1.0, 1.0, 0, 10, 25, 10, texture[8]);
    building(20, 0, -45, 1.0, 1.0, 1.0, 0, 10, 20, 10, texture[9]);    
    building(35, 0, -43, 1.0, 1.0, 1.0, 0, 10, 30, 5, texture[4]);   

    drawTree(30, 0, -25, 0.1, 0.1, 0.1, 270);
    drawTree(35, 0, -25, 0.1, 0.1, 0.1, 270);    
    drawTree(40, 0, -25, 0.1, 0.1, 0.1, 270);
    drawTree(25, 0, -25, 0.1, 0.1, 0.1, 270);
        
    drawTree(-25, 0, -45, 0.1, 0.1, 0.1, 270);
    drawTree(-20, 0, -45, 0.1, 0.1, 0.1, 270);
    drawTree(-15, 0, -45, 0.1, 0.1, 0.1, 270);    
    drawTree(-10, 0, -45, 0.1, 0.1, 0.1, 270); 
    drawTree(-5, 0, -45, 0.1, 0.1, 0.1, 270);    
              
    // Draw the flat land
    glPushMatrix();
    float scale1 = 5.0f / max(_terrain1->width() - 1, _terrain1->length() - 1);
	glScalef(20*scale1, 10*scale1, 20*scale1/3);
    glTranslatef(-(float)(_terrain3->width() - 1) / 2, 00.0f, -1.5*((float)(_terrain3->length() - 1)));	
    
	glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(int z = 0; z < _terrain1->length() - 1; z++) 
	{
		//Makes OpenGL draw a triangle at every three consecutive vertices
        glBindTexture(GL_TEXTURE_2D,texture[0]);
        glBegin(GL_QUAD_STRIP);
		for(int x = 0; x < _terrain1->width(); x++) 
		{
			Vec3f normal = _terrain1->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2f(x/100.0,z/100.0);
            glVertex3f(x, _terrain1->getHeight(x, z), z);
			normal = _terrain1->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2d(x/100.0, (z + 1)/100.0);
            glVertex3f(x, _terrain1->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    
    // Draw the middle grassland
    glPushMatrix();
    float scale2 = 5.0f / max(_terrain2->width() - 1, _terrain2->length() - 1);
	glScalef(20*scale2, 10*scale2, 20*scale2/3);
    glTranslatef(-(float)(_terrain2->width() - 1) / 2, 15.0f, -(float)(_terrain2->length() - 1)/2);	
    
	glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(int z = 0; z < _terrain2->length() - 1; z++) 
	{
		//Makes OpenGL draw a triangle at every three consecutive vertices
        glBindTexture(GL_TEXTURE_2D,texture[1]);
        glBegin(GL_QUAD_STRIP);
		for(int x = 0; x < _terrain2->width(); x++) 
		{
			Vec3f normal = _terrain2->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2f(x/100.0,z/100.0);
            glVertex3f(x, _terrain2->getHeight(x, z), z);
			normal = _terrain2->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2d(x/100.0, (z + 1)/100.0);
            glVertex3f(x, _terrain2->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Draw the mountains
    glPushMatrix();
    float scale3 = 5.0f / max(_terrain3->width() - 1, _terrain3->length() - 1);
	glScalef(20*scale2, 10*scale2, 20*scale3/3);
    glTranslatef(-(float)(_terrain3->width() - 1) / 2, 98.0f, ((float)(_terrain3->length() - 1))/2);	
    
	glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(int z = 0; z < _terrain3->length() - 1; z++) 
	{
		//Makes OpenGL draw a triangle at every three consecutive vertices
        glBindTexture(GL_TEXTURE_2D,texture[0]);
        glBegin(GL_QUAD_STRIP);
		for(int x = 0; x < _terrain3->width(); x++) 
		{
			Vec3f normal = _terrain3->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2f(x/100.0,z/100.0);
            glVertex3f(x, _terrain3->getHeight(x, z), z);
			normal = _terrain3->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2d(x/100.0, (z + 1)/100.0);
            glVertex3f(x, _terrain3->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    drawRoad(-50.0, 50.0, -30, -40, 0.01);
    drawDinosaur(-10.0, 15.0, 30.0, 0.04, 0.04, 0.04, 270);
    drawHouse(15.0, 0.0, -20.0, 0.2, 0.2, 0.2, 270);
    drawHumvee(15, 0, -25, 0.01, 0.01, 0.01, 90, 180);
    drawTree(20, 2, 10, 0.1, 0.1, 0.1, 270);
    drawTree(10, 2, 10, 0.1, 0.1, 0.1, 270);
    drawTree(-10, 2, 10, 0.1, 0.1, 0.1, 270);
    drawTree(-20, 2, 10, 0.1, 0.1, 0.1, 270);
    drawTree(5, 0, -8, 0.1, 0.1, 0.1, 270);
    drawTree(5, 0, -10, 0.1, 0.1, 0.1, 270);
    drawTree(-30, 0, -20, 0.1, 0.1, 0.1, 270);
    drawTree(30, 0, -15, 0.1, 0.1, 0.1, 270);    
    
    // No lighting from here on
    glDisable(GL_LIGHTING);
    
    
    
    glLineWidth(1);
    //  Draw axes
    glColor3f(0,0,0);
    if (axes)
    {
        glBegin(GL_LINES);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(len,0.0,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,len,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,0.0,len);
        glEnd();
        //  Label axes
        glRasterPos3d(len,0.0,0.0);
        Print("X");
        glRasterPos3d(0.0,len,0.0);
        Print("Y");
        glRasterPos3d(0.0,0.0,len);
        Print("Z");
    }
    
    //  Display parameters
    // glColor3f(0,0,0);
    glWindowPos2i(5,5);
    Print("FOV=%d", fov); 
    glWindowPos2i(5,25);
    Print("Ex:%.1f | Ey:%.1f | Ez:%.1f | lx:%.1f | lz:%.1f", Ex, Ey, Ez, lx, lz); 
    glWindowPos2i(5,45);
    Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f sco1=%.0f",ambient,diffuse,specular,emission,shinyvec[0], sco1);
    
    // Render the scene and make it visible
    // ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    // Look right
    if (key == GLUT_KEY_RIGHT)
    {
        angle -= 0.15f;
        lx = D*sin(angle);
        lz = D*cos(angle);        
    }
    // Look left
    else if (key == GLUT_KEY_LEFT)
    {
        angle += 0.15f;
        lx = D*sin(angle);
        lz = D*cos(angle);
    }
    // Move forward
    else if (key == GLUT_KEY_UP)
    {
        Ex += strafe_step_factor*D*lx;
        Ez += strafe_step_factor*D*lz;
    }
    // Move backward
    else if (key == GLUT_KEY_DOWN)
    {
        Ex -= strafe_step_factor*D*lx;
        Ez -= strafe_step_factor*D*lz;
    }
    // Raise viewing height
    else if (key == GLUT_KEY_PAGE_UP)
        Ey += 0.5;
    // Lower viewing height
    else if (key == GLUT_KEY_PAGE_DOWN && Ey>1)
        Ey -= 0.5;        
    // Smooth or Flat shading
    else if (key == GLUT_KEY_F1)
        smooth = 1-smooth;
    // Toggle two sided lighting
    else if (key == GLUT_KEY_F3)
        side = 1-side;

    // Update projection
    Project(fov,asp,dim);
    // Tell GLUT it is necessary to redisplay the scene
    
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360);
    if (move ==1) Th = fmod(20*t,360);
    else Th = 135;
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    // Exit on ESC
    if (ch == 27)
        exit(0);
    // Reset view angle
    else if (ch == '0')
    {
        Ex = 49.0; Ey = 11.0; Ez = -39.0; lx = -4.0; lz = 3.0;
    }
    // Dino's perspective
    else if (ch == '1')
    {
        Ex = -4.0; Ey = 30.0; Ez = 42.0; lx = -1.0; lz = -5.0;
    }
    // Side view point
    else if (ch == '2')
    {
        Ex = 62.5; Ey = 11.0; Ez = 1.1; lx = -4.9; lz = -1.1;
    }
    // Building top view point
    else if (ch == '3')
    {
        Ex = -49.8; Ey = 29.5; Ez = -43.7; lx = 3.3; lz = 3.7;
    }
    // Toggle axes
    else if (ch == 'q' || ch == 'Q')
        axes = 1-axes;
    // Scope cutoff for helicopter spotlight
    else if (ch == 'R')
        sco1++;
    else if (ch == 'r')
        sco1--;
    // Toggle Helicopter and heli-spotlight
    else if (ch == 'h' || ch == 'H')
        spotlight = 1-spotlight;
    // Spotlight movement
    else if (ch == '<')
        Sx+=2;
    else if (ch == '>')
        Sx-=2;
    else if (ch == ',')
        Sz+=2;        
    else if (ch == '.')
        Sz-=2;                    
    // Change field of view angle
    else if (ch == '-' && ch>1)
        fov--;
    else if (ch == '+' && ch<179)
        fov++;
    // Toggle lighting
    else if (ch == 'l' || ch == 'L')
        light = 1-light;   
    //  Toggle light movement
    else if (ch == 'o' || ch == 'O')
        move = 1-move;        
    //  Ambient level
    else if (ch=='a' && ambient>0)
        ambient -= 5;
    else if (ch=='A' && ambient<100)
        ambient += 5;
    // Diffuse level
    else if (ch=='d' && diffuse>0)
        diffuse -= 5;
    else if (ch=='D' && diffuse<100)
        diffuse += 5;
    //  Specular level
    else if (ch=='s' && specular>0)
        specular -= 5;
    else if (ch=='S' && specular<100)
        specular += 5;
    //  Emission level
    else if (ch=='e' && emission>0)
        emission -= 5;
    else if (ch=='E' && emission<100)
        emission += 5;
    //  Shininess level
    else if (ch=='n' && shininess>-1)
        shininess -= 1;
    else if (ch=='N' && shininess<7)
        shininess += 1;
   // Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);

    // Reproject
    Project(fov,asp,dim);
    // Idle function modified to toggle light movement
    // glutIdleFunc(move?idle:NULL);
    glutIdleFunc(idle);
    // Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024,768);
    //  Create the window
    glutCreateWindow("Final Project: Swapnil Paratey");
    //  Tell GLUT to call "idle" when there is nothing else to do
    glutIdleFunc(idle); 
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    // glutSpecialUpFunc(specialUp);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    
    //  Load textures
  	_terrain1 = loadTerrain("h.bmp", 0);
  	_terrain2 = loadTerrain("h.bmp", 30);
  	_terrain3 = loadTerrain("h.bmp", 200);
	texture[0] = LoadTexBMP("sand.bmp");  	
    texture[1] = LoadTexBMP("grassland.bmp");
    texture[2] = LoadTexBMP("redirect.bmp"); 
    texture[3] = LoadTexBMP("road.bmp");
    texture[4] = LoadTexBMP("buildingwindow.bmp");
    texture[5] = LoadTexBMP("redbuilding.bmp");
    texture[6] = LoadTexBMP("brick.bmp");
    texture[7] = LoadTexBMP("helipad.bmp");   
    texture[8] = LoadTexBMP("oldbuilingtextx.bmp");     
    texture[9] = LoadTexBMP("brownwindow.bmp");    
    dino = LoadOBJ("TRexModel.obj");   
    house = LoadOBJ("house.obj");
    tree = LoadOBJ("Palm_Tree.obj");
    humvee = LoadOBJ("humvee.obj");
    heli = LoadOBJ("NewsHelicopter.obj");
    
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
}
