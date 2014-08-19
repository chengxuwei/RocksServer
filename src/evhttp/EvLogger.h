/**
 *  EvLogger.h
 *
 *  Class for logging evhttp
 *
 *  This class contains a static field.
 *  Unfortunately, due to the architecture of libevent these field can't be removed.
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */



namespace RocksServer {

    class EvLogger
    {

    public: 
        // none | debug | msg |  warn | error | fatal 
        enum class Level : int {
            debug  = EVENT_LOG_DEBUG,
            msg    = EVENT_LOG_MSG,
            warn   = EVENT_LOG_WARN,
            error  = EVENT_LOG_ERR,
            fatal,
            none
        };

        EvLogger(const Level &log_level, const std::string &filename) 
        {
            _logLevel = log_level;

            // if log level: none, nothing required
            if(Level::none == log_level){
                return;
            }

            _file.open(filename, std::ios_base::app);


            // Turn on debug mode
            if(Level::debug == log_level){
                event_enable_debug_mode();
            }

            event_set_fatal_callback([](int err){
                std::string msg = "fatal error: " + std::to_string(err);
                EvLogger::write("fatal", msg.c_str());
                exit(1);
            });
            
            // If only fatal errors, event_set_log_callback not required
            if(Level::fatal == log_level){
                return;
            }
            
            event_set_log_callback(EvLogger::writeLog);

        }

    	~EvLogger()
        {
            if(_file) _file.close();
        }

        /**
         *  Cast to a bool 
         *  For check if logger successfully created
         */
        operator bool () const
        //bool isUsable() const
        {
            return (_logLevel == Level::none) || _file;
        }


    private:

        //static void write(int severity, std::string msg)
        static void writeLog(int severity, const char * msg)
        {
            
            // Filtering errors by level
            if( severity < static_cast<int>(_logLevel) ) {
                return;
            }

            std::string severityName;
            switch (severity) {
                case static_cast<int>(Level::debug):
                    severityName = "debug";
                    break;
                case static_cast<int>(Level::msg):
                    severityName = "msg";
                    break;
                case static_cast<int>(Level::warn):
                    severityName = "warn";
                    break;
                case static_cast<int>(Level::error):
                    severityName = "error";
                    break;
                default:
                    severityName = "???";
                    break;
            }

            write(severityName, msg);
        }

        static void write(const std::string &severity, const char * msg)
        {
            // see http://www.cplusplus.com/reference/ctime/tm/
            time_t now = time(NULL);
            auto t = localtime(&now);

            _file   << 1900+t->tm_year << "/" 
                    << t->tm_mon+1 << "/" 
                    << t->tm_mday << " " 
                    << t->tm_hour << ":" 
                    << t->tm_min << ":" 
                    << t->tm_sec << " [" << severity << "] " << msg << std::endl;
        }

        static std::ofstream _file;

        static Level _logLevel;

    };

    std::ofstream EvLogger::_file;
    EvLogger::Level EvLogger::_logLevel;


}
