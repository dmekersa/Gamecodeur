#ifndef ESCENEPARSER_H
#define ESCENEPARSER_H
#include <list>
#include <map>

namespace ESceneParser {
   void strip(std::string& str);

   std::map< std::string, std::string > typedef SceneObjectMap;
   std::list< SceneObjectMap > typedef SceneObjectList;
   std::map< std::string, std::string >::iterator typedef ObjectIterator;
   std::list< SceneObjectMap >::iterator typedef SceneObjectIterator;

   void ParseLine(SceneObjectMap& obj, std::string& line);
   void ParseScene(SceneObjectList& objList, const char *ScenePath);
   
   const char* GetValue(SceneObjectMap& obj, const char *key);
   const char* GetValue(SceneObjectIterator& obj, const char *key);
   const std::string* GetStrValue(SceneObjectMap& obj, const char *key);
   const std::string* GetStrValue(SceneObjectIterator& obj, const char *key);

   class RTextFile {
   public:
      RTextFile(const char *path);
      ~RTextFile();

      bool IsOpen();
      bool eof();
      const char* GetNextStrToken(std::string &str);
      bool GetLine(std::string &str);
      
   private:
      char *_buff, *_curpos;
      size_t _size;
      std::string _filePath;
   };
}
#endif