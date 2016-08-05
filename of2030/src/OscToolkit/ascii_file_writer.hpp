//
//  ascii_file_writer.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#ifndef ascii_file_writer_hpp
#define ascii_file_writer_hpp

#include "osc_interface.hpp"

namespace OscToolkit{

    class AsciiFileWriter : public of2030::OscInterface {

    public: // methods
        // AsciiFileWriter(){}
        ~AsciiFileWriter(){ destroy(); }
        void destroy();

        // writing
        void start(string path);
        void stop();
        
        // overwrite parent class' implementation
        void process(ofxOscMessage &message);

    private: // writing attributes

        float start_time, first_message_time;
        std::ofstream outfile;
    };

} // OscToolkit


#endif /* ascii_file_writer_hpp */
