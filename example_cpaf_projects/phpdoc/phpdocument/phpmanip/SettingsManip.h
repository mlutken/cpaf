#ifndef PHPDOC_SETTINGSMANIP_H
#define PHPDOC_SETTINGSMANIP_H

namespace phpdoc {

class File;

class SettingsManip
{
    public:
    explicit SettingsManip  (File* f);



    private:
    File*   m_file;
};

} // namespace phpdoc

#endif // PHPDOC_SETTINGSMANIP_H
