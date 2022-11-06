#include "testDefines.h"

#include "../db/db.h"
#include "../db/sqlDefines.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

void resetAll();

// Inits db
void init() {
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tmp.sqlite"); // FIXME
    db.open();

    QSqlQuery("PRAGMA foreign_keys=OFF");

    resetAll();
}

// Return list of ids of themes from themes
QList<int> toIds(const QList<Theme>& themes) {
    QList<int> ans;
    ans.reserve(themes.size());
    for (const auto& theme : themes) {
        ans.append(theme.id);
    }
    return ans;
}

// Resets packages db table and inserts sample data
void resetPackages() {
    QSqlQuery("DROP TABLE IF EXISTS packages");
    QSqlQuery(" \
        CREATE TABLE packages( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE \
        ) \
    ");
    QSqlQuery(" \
        INSERT \
        INTO packages \
        VALUES \
            (1, 'p1'), \
            (2, 'p2') \
    ");
}

// Resets themes db table and inserts sample data
void resetThemes() {
    QSqlQuery("DROP TABLE IF EXISTS themes");
    QSqlQuery(" \
        CREATE TABLE themes( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL, \
            packageId INT NOT NULL REFERENCES packages(id), \
            description TEXT, \
            inWishlist INT CHECK(inWishlist in (0, 1)), \
            isLearned INT CHECK(isLearned in (0, 1)), \
            UNIQUE (packageId, name) \
        ) \
    ");
    QSqlQuery(" \
        INSERT \
        INTO themes  \
        VALUES \
            (1,'a',1,'',1,0), \
            (2,'b',1,'',0,1), \
            (3,'c',1,'',0,0), \
            (4,'d',1,'',1,1), \
            (5,'t',1,'',1,1), \
            (6,'t1',2,'',1,0), \
            (7,'t2',2,'',0,1), \
            (8,'t3',2,'',0,0), \
            (9,'t4',2,'',1,1) \
    ");
}

// Resets themeEdges db table and inserts sample data
void resetThemeEdges() {
    QSqlQuery("DROP TABLE IF EXISTS themeEdges");
    QSqlQuery(" \
        CREATE TABLE themeEdges( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            beginId INT NOT NULL REFERENCES themes(id), \
            endId INT NOT NULL REFERENCES themes(id), \
            UNIQUE (beginId, endId), \
            CHECK (beginId != endId) \
        ) \
    ");
    QSqlQuery(" \
        INSERT \
        INTO themeEdges \
        VALUES \
            (1,1,2), \
            (2,1,3), \
            (3,3,4), \
            (4,2,4), \
            (5,5,7), \
            (6,6,5), \
            (7,7,8), \
            (8,8,6) \
    ");
}

// Resets graphs db table and inserts sample data
void resetGraphs() {
    QSqlQuery("DROP TABLE IF EXISTS graphs");
    QSqlQuery(" \
        CREATE TABLE graphs( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            name VARCHAR(255) NOT NULL UNIQUE \
        ) \
    ");
    QSqlQuery(" \
        INSERT \
        INTO graphs \
        VALUES \
            (1, 'g1'), \
            (2, 'g2') \
    ");
}

// Resets db and inserts sample data
void resetAll() {
    resetPackages();
    resetThemes();
    resetGraphs();
    resetThemeEdges();
}

int main() {
    init();

    Theme t1, t2;
    Package p;
    Graph g;

    TEST_GROUP_BEGIN("package renaming")

    p.id = 1;
    p.name = "pack";
    db::package::write(p);
    TEST_COMPARE(db::package::name(p.id), p.name)
    resetPackages();

    p.id = 1;
    p.name = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    db::package::write(p);
    TEST_COMPARE(db::package::name(p.id), p.name)
    resetPackages();

    p.id = 1;
    p.name = "";
    try {
        db::package::write(p);
        TEST_FAILED
    } catch (const QString&) {
        TEST_PASSED
    } catch (...) {
        TEST_FAILED
    }
    resetPackages();

    p.id = 1;
    p.name = "p2";
    try {
        db::package::write(p);
        TEST_FAILED
    } catch (const QString&) {
        TEST_PASSED
    } catch (...) {
        TEST_FAILED
    }
    resetPackages();

    TEST_GROUP_END



    TEST_GROUP_BEGIN("graph renaming")

    g.id = 1;
    g.name = "graph";
    db::graph::write(g);
    TEST_COMPARE(db::graph::name(g.id), g.name)
    resetGraphs();

    g.id = 1;
    g.name = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    db::graph::write(g);
    TEST_COMPARE(db::graph::name(g.id), g.name)
    resetGraphs();

    g.id = 1;
    g.name = "";
    try {
        db::graph::write(g);
        TEST_FAILED
    } catch (const QString&) {
        TEST_PASSED
    } catch (...) {
        TEST_FAILED
    }
    resetGraphs();

    g.id = 1;
    g.name = "g2";
    try {
        db::graph::write(g);
        TEST_FAILED
    } catch (const QString&) {
        TEST_PASSED
    } catch (...) {
        TEST_FAILED
    }
    resetGraphs();

    TEST_GROUP_END



    TEST_GROUP_BEGIN("theme filters")

    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1, 2, 3, 4, 5})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            -1,
            Qt::PartiallyChecked,
            Qt::Unchecked
        )),
        (QList<int>{1, 3, 6, 8})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "a",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "th",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{5, 6, 7, 8, 9})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            1,
            Qt::Checked,
            Qt::Checked
        )),
        (QList<int>{5})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            1,
            Qt::PartiallyChecked,
            Qt::Checked
        )),
        (QList<int>{5})
    )

    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            1,
            Qt::Unchecked,
            Qt::Checked
        )),
        (QList<int>{})
    )

    TEST_GROUP_END



    TEST_GROUP_BEGIN("creating theme")

    t1.id = -1;
    t1.name = "theme";
    t1.package.id = 1;
    t1.description = "description";
    t1.isLearned = true;
    t1.inWishlist = false;
    t1.id = db::theme::write(t1);
    t2 = db::theme::read(t1.id);
    TEST_CHECK(
        t1.name == t2.name &&
        t1.package.id == t2.package.id &&
        db::package::name(t1.package.id) == t2.package.name &&
        t1.description == t2.description &&
        t1.isLearned == t2.isLearned &&
        t1.inWishlist == t2.inWishlist
    )
    resetThemes();

    t1.id = -1;
    t1.name = "";
    t1.package.id = 1;
    try {
        db::theme::write(t1);
        TEST_FAILED
    } catch(const QString&) {
        TEST_PASSED
    } catch(...) {
        TEST_FAILED
    }
    resetThemes();

    t1.id = -1;
    t1.name = "a";
    t1.package.id = 1;
    try {
        db::theme::write(t1);
        TEST_FAILED
    } catch(const QString&) {
        TEST_PASSED
    } catch(...) {
        TEST_FAILED
    }
    resetThemes();

    TEST_GROUP_END



    TEST_GROUP_BEGIN("package and theme delition")

    QSqlQuery q1, q2, q3;

    db::theme::del(1);
    PREPARE(q1, "SELECT COUNT(*) FROM themes")
    PREPARE(q2, "SELECT COUNT(*) FROM themeEdges")
    EXEC(q1)
    EXEC(q2)
    q1.next();
    q2.next();
    TEST_CHECK(
        q1.value(0).toInt() == 8 &&
        q2.value(0).toInt() == 6
    )
    q1.finish();
    q2.finish();
    resetAll();

    db::package::del(1);
    db::theme::del(1);
    PREPARE(q1, "SELECT COUNT(*) FROM packages")
    PREPARE(q2, "SELECT COUNT(*) FROM themes")
    PREPARE(q3, "SELECT COUNT(*) FROM themeEdges")
    EXEC(q1)
    EXEC(q2)
    EXEC(q3)
    q1.next();
    q2.next();
    q3.next();
    TEST_CHECK(
        q1.value(0).toInt() == 1 &&
        q2.value(0).toInt() == 4 &&
        q3.value(0).toInt() == 2
    )
    q1.finish();
    q2.finish();
    q3.finish();
    resetAll();

    TEST_GROUP_END
}
