#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>			// added in VC++
#include <time.h>		// added in VC++
using namespace std;

#define MAX_CACHE_COUNT       50

class File {
  public :
    virtual int GetFile(string fn, void *pOut) = 0;
};

class ImageFile : public File {
  public :
    int GetFile(string fn, void *pOut) {
      struct stat statBuf;
      if (stat(fn.data(), &statBuf) < 0) {
        cout << "File Not Found" << endl;
        return (0);
      }

      int fd = _open(fn.data(), O_RDONLY);		// changed to _open in VC++
      if (fd < 0) {
        cout << "File Open Error" << endl;
        return (0);
      }

      pOut = new char[statBuf.st_size];
      int byteCnt = _read(fd, pOut, statBuf.st_size);	// changed to _read in VC++
      _close(fd);		// changed to _close in VC++

      if (byteCnt != statBuf.st_size) {
        cout << "Error while File Reading" << endl;
        return(0);
      }
      return(byteCnt);
    }
};

class ImageFileCache : public File {
  public :
    int GetFile(string fn, void *pOut) {
      pOut = fileCache_[fn];

      struct stat realStat;
      if (stat(fn.data(), &realStat) < 0) {
        cout << "File Not Found" << endl;
        return(0);
      }

      struct stat *pFileStat = fileStat_[fn];
      if (pOut == NULL || pFileStat == 0 
                        || realStat.st_mtime != pFileStat->st_mtime) {
        // -- 캐싱이 안 되어 있거나, 캐싱된 정보가 실제 파일과 다를때
        ImageFile f;
        void* pFileOut;
        int fileSize = f.GetFile(fn, pFileOut);

        if (fileSize <= 0) {
          pOut = NULL;
          return (0);
        }

        RegisterCache(fn, &realStat, pFileOut);
        pOut = pFileOut;
      }
      else {
        // -- 최근 읽은 시간 수정
        for (int i=0; i < MAX_CACHE_COUNT; i++) {
          if (lruInfo_[i].fn_ == fn) {
            lruInfo_[i].lastReadTime_ = time(0);
            break;
          }
        }
      }

      return(realStat.st_size);
    }

  protected :
    void RegisterCache(string fn, struct stat* pFileStat, void* pFile) {
      int cachePos = 0;
      time_t oldestReadTime = time(0);
      for (int i=0; i < MAX_CACHE_COUNT; i++) {
        if (lruInfo_[i].lastReadTime_ == 0) {
          // -- cache 공간이 남아 있는 상태
          cachePos = i;
          break;
        }
        else if (oldestReadTime > lruInfo_[i].lastReadTime_) {
          cachePos = i;
          oldestReadTime = lruInfo_[i].lastReadTime_;
        }
        else {}
      }

      if (lruInfo_[cachePos].lastReadTime_ != 0) {
        // -- 이전 캐싱 정보 삭제
        struct stat *pOldFileStat = fileStat_[lruInfo_[cachePos].fn_];
        void *pOldImgFile = fileCache_[lruInfo_[cachePos].fn_];

        delete fileStat_[lruInfo_[cachePos].fn_];
        delete fileCache_[lruInfo_[cachePos].fn_];

        fileStat_.erase(lruInfo_[cachePos].fn_);
        fileCache_.erase(lruInfo_[cachePos].fn_);
      }

      lruInfo_[cachePos].fn_ = fn;
      lruInfo_[cachePos].lastReadTime_ = time(0);

      struct stat* pNewStat = new struct stat;
      // bcopy(pFileStat, pNewStat, sizeof(struct stat));
	  memcpy(pNewStat, pFileStat, sizeof(struct stat));		// changed to memcpy in VC++

      fileStat_[fn] = pNewStat;
      fileCache_[fn] = (char *)pFile;
    }

  private :
    class LRUInfo {
      public : 
        LRUInfo() { lastReadTime_ = 0; }
        string fn_;
        time_t lastReadTime_;
    }; 

    LRUInfo lruInfo_[MAX_CACHE_COUNT];
    map<string, struct stat*> fileStat_;
    map<string, char*> fileCache_;
};


void
main()
{
  File *pFileServer = new ImageFileCache;

  void *pImgData;
  int fileSize = pFileServer->GetFile("./comic/img/hero1.jpg", pImgData);
}
