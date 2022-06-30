#include <QWidget>
#include <QString>

class LearningListWindow : public QWidget {
    public:
        LearningListWindow(const QString& themeName, QWidget* parent = nullptr);

    private:
        QString themeName;
};
