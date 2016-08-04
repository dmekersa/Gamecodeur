#include "GraphicalStringTrad.h"
#include "global.h"
#include "EFancyString.h"

GraphicalStringTrad::GraphicalStringTrad() 
: x(0.0f), y(0.0f), m_visible(true), m_freezeColor(false), m_pFont(NULL), m_pRenderer(NULL) {
   
   setColor(1.0f, 1.0f, 1.0f);
}

GraphicalStringTrad::~GraphicalStringTrad() {
   XDELETE(m_pRenderer);
}

void GraphicalStringTrad::render() {
   
   if(m_pRenderer)
      m_pRenderer->render();
}

EFancyString* GraphicalStringTrad::getRenderer() {
   return m_pRenderer;
}

void GraphicalStringTrad::setRenderer(EFancyString* fancyString) {
   XDELETE(m_pRenderer);
   m_pRenderer = fancyString;
   m_pRenderer->setColor(m_pcolor);
}

void GraphicalStringTrad::setFreezeColor(bool b) {
   m_freezeColor = b;
}

void GraphicalStringTrad::setColor(float r, float g, float b) {

   if(m_freezeColor)
      return;

   m_pcolor[0] = r;
   m_pcolor[1] = g;
   m_pcolor[2] = b;

   if(m_pRenderer)
      m_pRenderer->setColor(m_pcolor);
}

const float* GraphicalStringTrad::getColor() const {
   return m_pcolor;
}

float GraphicalStringTrad::r() const {
   return m_pcolor[0];
}

float GraphicalStringTrad::g() const {
   return m_pcolor[1];
}

float GraphicalStringTrad::b() const {
   return m_pcolor[2];
}

const std::string &  GraphicalStringTrad::getString() const {
   return m_sz;
}

void GraphicalStringTrad::setString(const std::string & string) {
   m_sz = string;
}

void GraphicalStringTrad::setString(const char* string) {
   m_sz = std::string(string);
}

void GraphicalStringTrad::setTrad(const std::string & string) {
   
   m_trad = string;
/*   m_vectorTrad.clear();

   int indexReturn = 0;
   int posString = 0;
   while(indexReturn != std::string::npos) {
      indexReturn = string.find_first_of('\n', posString);
      m_vectorTrad.push_back(string.substr(posString, indexReturn-posString));
      posString = indexReturn+1;
   }
*/
}

const std::string & GraphicalStringTrad::getTrad() const {
   return m_trad;
}

void GraphicalStringTrad::setTrad(const char * string) {
   if(string)
      setTrad(std::string(string));
}

bool GraphicalStringTrad::isVisible() const {
   return m_visible;
}

void GraphicalStringTrad::setVisible(bool visible) {
   m_visible = visible;
}