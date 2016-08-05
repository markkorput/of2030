//
//  ascii_file_reader.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#ifndef ascii_file_reader_hpp
#define ascii_file_reader_hpp

#include "ascii_file_line.hpp"

namespace OscToolkit {

    class AsciiFileReader {

    public: // methods
        AsciiFileReader();
        ~AsciiFileReader(){ destroy(); }
        void destroy();

        // reading
        bool load(string path);
        AsciiFileLine* next_line();
        inline const string &getReadPath(){ return readpath; }
        inline void reset(){
            load(readpath);
        }

        inline void setLoop(bool loop){ bLoop = loop; }
        
    private:
        AsciiFileLine* read_line();
        
    private: // reading attributes
        AsciiFileLine last_line;
        std::ifstream infile;
        bool bLoop;
        string readpath;
    };

} // namespace OscToolkit

#endif /* ascii_file_reader_hpp */
