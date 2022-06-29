#include <QWidget>
#include <QString>

class ThemeInfo : public QWidget {
    public:
        ThemeInfo(const QString& themeName, QWidget* parent = nullptr);

    private:
        QString themeName;
};
