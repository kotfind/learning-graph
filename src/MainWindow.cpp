#include "MainWindow.h"

#include "LearningListWidget.h"
#include "ThemeTab.h"
#include "PackageTab.h"
#include "GlobalSignalHandler.h"

#include <QTabWidget>
#include <QMessageBox>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QFontDatabase>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent) {
    setWindowTitle(tr("Learning Graph"));

    // Load settings
    QSettings settings;
    auto font = qvariant_cast<QFont>(settings.value("font", qApp->font()));
    qApp->setFont(font);
    emit GlobalSignalHandler::getInstance()->fontSet(font);

    ui();

    connect(
        graphsTab,
        &GraphTab::open,
        graphEditWidget,
        &GraphEditWidget::open
    );

    connect(
        fontSizeActionGroup,
        &QActionGroup::triggered,
        this,
        &MainWindow::onFontSizeActionTriggered
    );

    connect(
        languageActionGroup,
        &QActionGroup::triggered,
        this,
        &MainWindow::onLanguageActionTriggered
    );
}

void MainWindow::ui() {
    uiHeader();
    uiBody();
}

void MainWindow::uiHeader() {
    auto* settingsMenu = menuBar()->addMenu(tr("Settings"));

    // Fonts
    auto* fontSizeMenu = settingsMenu->addMenu(tr("Font Size"));

    fontSizeActionGroup = new QActionGroup(this);
    fontSizeActionGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

    auto font = qApp->font();
    qApp->setFont(font);
    emit GlobalSignalHandler::getInstance()->fontSet(font);

    for (int size : QFontDatabase::pointSizes(qApp->font().family())) {
        auto* action = new QAction(QString::number(size), this);

        action->setCheckable(true);
        action->setChecked(size == qApp->font().pointSize());
        action->setData(size);

        // NOTE: idk why it won't work:
        // auto font = qApp->font();
        // font.setPointSize(size);
        // action->setFont(font);

        fontSizeActionGroup->addAction(action);
        fontSizeMenu->addAction(action);
    }

    // Language
    auto* languageMenu = settingsMenu->addMenu(tr("Language"));

    languageActionGroup = new QActionGroup(this);
    languageActionGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);

    QSettings settings;
    const auto lang = settings.value("locale").toString();

    auto* englishLanguageAction = new QAction(tr("English"), this);
    englishLanguageAction->setCheckable(true);
    englishLanguageAction->setChecked(lang == "en");
    englishLanguageAction->setData("en");
    languageActionGroup->addAction(englishLanguageAction);
    languageMenu->addAction(englishLanguageAction);

    auto* russianLanguageAction = new QAction(tr("Russian"), this);
    russianLanguageAction->setCheckable(true);
    russianLanguageAction->setChecked(lang == "ru");
    russianLanguageAction->setData("ru");
    languageActionGroup->addAction(russianLanguageAction);
    languageMenu->addAction(russianLanguageAction);
}

void MainWindow::uiBody() {
    auto* splitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(splitter);

    // Left Tabs
    auto* leftTabs = new QTabWidget;
    splitter->addWidget(leftTabs);

    auto* themesTab = new ThemeTab;
    leftTabs->addTab(themesTab, tr("Themes"));

    auto* packagesTab = new PackageTab;
    leftTabs->addTab(packagesTab, tr("Packages"));

    graphsTab = new GraphTab;
    leftTabs->addTab(graphsTab, tr("Graphs"));

    // Graph Edit tab
    graphEditWidget = new GraphEditWidget;
    splitter->addWidget(graphEditWidget);

    // Learning List tab
    learningListWidget = new LearningListWidget;
    splitter->addWidget(learningListWidget);

    // Splitter stretch
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
}

void MainWindow::onFontSizeActionTriggered(QAction* action) {
    auto font = qApp->font();
    font.setPointSize(action->data().toInt());
    qApp->setFont(font);
    emit GlobalSignalHandler::getInstance()->fontSet(font);

    QSettings settings;
    settings.setValue("font", font);
}

void MainWindow::onLanguageActionTriggered(QAction* action) {
    QSettings settings;
    if (settings.value("locale") != action->data()) {
        settings.setValue("locale", action->data());

        QMessageBox::information(
            this,
            tr("Info"),
            tr("Restart the application for changes to take effect.")
        );
    }
}
