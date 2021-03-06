/**
 *  BackupEngine.h
 *
 *  Wrapper for backup and restore DB
 *
 *  More info: http://rocksdb.org/blog/191/how-to-backup-rocksdb/
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */


namespace RocksServer 
{
    class BackupEngine
    {
    
    public:
        
        /**
         * Constructor
         * @param   location of DB backup
         */
        BackupEngine(const std::string &bk_path) :
            _engine( rocksdb::BackupEngine::NewBackupEngine(rocksdb::Env::Default(), rocksdb::BackupableDBOptions(bk_path)) ) {}

        virtual ~BackupEngine()
        {
            delete _engine;
        }

        /**
         * Constructor
         * @param string path
         * @param location of DB log files
         */
        rocksdb::Status createBackup(rocksdb::DB* db) const
        {
            return _engine->CreateNewBackup(db);
        }

        /**
         * Restore DB from latest backup
         * @param       db_dir  location of DB
         * @param       wal_dir location of DB log files
         */
        rocksdb::Status restoreBackup(const std::string& db_dir, const std::string& wal_dir) const
        {
            return _engine->RestoreDBFromLatestBackup(db_dir, wal_dir);
        }

        /**
         * Restore DB from latest backup
         * @param       db_dir  location of DB
         */
        rocksdb::Status restoreBackup(const std::string& db_dir) const
        {
            return _engine->RestoreDBFromLatestBackup(db_dir, db_dir);
        }

        /**
         * Restore DB from backup by index
         * @param       db_dir  location of DB
         * @param       index   backup index
         */
        rocksdb::Status restoreBackup(const std::string& db_dir, uint32_t index) const
        {
            return _engine->RestoreDBFromBackup(index, db_dir, db_dir);
        }

        /**
         * Get backups list information
         */
        std::vector<rocksdb::BackupInfo> backupInfo() const
        {
            std::vector<rocksdb::BackupInfo> backup_info;
            _engine->GetBackupInfo(&backup_info);
            return backup_info;
        }
        
    private:
        rocksdb::BackupEngine* _engine;
    };

}