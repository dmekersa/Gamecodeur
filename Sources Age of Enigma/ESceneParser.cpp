#include <sstream>
#include <fstream>
#include <string>
#include "KPTK.h"
#include "ESceneParser.h"
#include "global.h"

size_t mystrcmp(const char *s1, register const char *s2, register char *endparse)
{
   register const char *p, *spanp;
   register char c, sc;

   /*
   * Stop as soon as we find any character from s2.  Note that there
   * must be a NUL in s2; it suffices to stop when we find that, too.
   */
   for (p = s1;;) {
      c = *p++;
      spanp = s2;
      do {
         if ((sc = *spanp++) == c)
            return (p -1 - s1);
         if ( p >= endparse)
            return (p - s1);
      } while (sc != 0);
   }
   /* NOTREACHED */
}

namespace ESceneParser {


   RTextFile::RTextFile(const char *path) {
      _buff = NULL;
      EGLL();
      bool success = KResource::loadResource(path,(unsigned char *&)_buff,_size);
      EGLR();
      if (!success)
          K_LOG("ESceneParser -- Unable to parse scene: %s",path);
      _curpos = _buff;
   }
   RTextFile::~RTextFile() {
      if(_buff != NULL) {
         free(_buff);
         _buff = NULL;
      }
      _size = 0;
   }

   bool RTextFile::IsOpen() {
      return _buff != NULL;
   }
   bool RTextFile::eof() {
      if (!IsOpen())
         return true;
      return (_curpos-_buff >= _size);
   }
   const char *RTextFile::GetNextStrToken(std::string &str) {
      char *set = "\r\t\n ";
      char *strstart;
      str="";

      // Strip la classe blank avant le nouveau token;
      while (_curpos < _buff+_size && (*_curpos == '\r' || *_curpos == '\n' || *_curpos == ' ' || *_curpos == '\t'))
         _curpos++;
      strstart = _curpos;
      
      if (!eof()) {
         int next = mystrcmp(_curpos,set,_buff+_size);
         str.assign(_curpos,_curpos+next);
         _curpos = _curpos+next;
      }

      return str.c_str();
   }


   bool RTextFile::GetLine(std::string &str) {
      if (eof())
         return false;

      char endofline[] = "\n\r";
      int next = mystrcmp(_curpos,endofline,_buff+_size);
      if (_curpos+next - _buff >= _size)
         next = _buff+_size-_curpos;
      str.assign(_curpos,next);
      _curpos = _curpos+next;
      while ( _curpos - _buff < _size && (*_curpos == '\r' || *_curpos == '\n' ) )
         _curpos++;
      return true;
   }

   void strip(std::string& str) {
      str.erase(str.find_last_not_of(" ") + 1);	//RTrim
      str.erase(0, str.find_first_not_of(" "));	//LTrim
   }

   void ParseLine(SceneObjectMap& obj, std::string& line) {
      std::string key, value;
      obj.clear();

      if (line[0] == '#') // suppress comment line
         return;

      const char *start = line.c_str(), *end, *last=line.c_str() + strlen(line.c_str());

      while (start < last) {
         end = strchr(start,':');
         if (end == NULL)
            break;
         key.assign(start,end);
         strip(key);
         start = end+1;

         end = strchr(start,'|');
         if (end == NULL)
            end=last;
         value.assign(start,end);
         strip(value);
         start = end+1;

         obj[key] = value;
      }
   }

   void ParseScene(SceneObjectList& objList, const char *ScenePath) {
      objList.clear();

      std::string line;
      RTextFile scenefile(ScenePath);

      K_LOG("ESceneParser -- Start parsing: %s",ScenePath);
      SceneObjectMap obj;
      while (scenefile.GetLine(line)) {
         ParseLine(obj,line);
         if (obj.size() > 0)
            objList.push_back(obj);
      }
      K_LOG("ESceneParser -- End parsing: %s",ScenePath); 
   }

   const std::string* GetStrValue(SceneObjectMap& obj, const char *key) {
      ObjectIterator it = obj.find(key);
      if(it != obj.end())
         return &(it->second);
      return NULL;
   }
   
   const std::string* GetStrValue(SceneObjectIterator& obj, const char *key) {
      return GetStrValue((*obj), key);
   }

   const char* GetValue(SceneObjectMap& obj, const char *key) {
      ObjectIterator it = obj.find(key);
      if(it != obj.end())
         return it->second.c_str();
      return NULL;
   }
   const char* GetValue(SceneObjectIterator& obj, const char *key) {
      return GetValue((*obj),key);
   }
}