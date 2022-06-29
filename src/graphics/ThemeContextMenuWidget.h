#include <QMenu>
#include <QString>

class ThemeContextMenuWidget : public QMenu {
    Q_OBJECT

    public:
        ThemeContextMenuWidget(const QString& themeName, QWidget* parent = nullptr);

    private:
        QString themeName;
};
