#include "testDefines.h"

#include "../db/db.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

void resetDb();

// Inits db
void init() {
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tmp.sqlite"); // FIXME
    db.open();

    resetDb();
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

// Resets db and inserts sample data
void resetDb() {
    // Packages
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

    // Themes
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

int main() {
    init();

    Package p;

    // TEST GROUP
    TEST_GROUP_BEGIN("package renaming")

    // TEST
    p.id = 1;
    p.name = "pack";
    db::package::write(p);
    TEST_COMPARE(db::package::name(p.id), p.name)
    resetDb();

    // TEST
    p.id = 1;
    p.name = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    db::package::write(p);
    TEST_COMPARE(db::package::name(p.id), p.name)
    resetDb();

    // TEST
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
    resetDb();

    // TEST
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
    resetDb();

    TEST_GROUP_END



    // TEST GROUP
    TEST_GROUP_BEGIN("theme filters")

    // TEST
    qDebug() <<
        db::theme::reads(
            "",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        ).size();
    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1, 2, 3, 4, 5, 6, 7, 8, 9})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1, 2, 3, 4, 5})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            -1,
            Qt::Checked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1, 4, 5, 6, 9})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "",
            -1,
            Qt::PartiallyChecked,
            Qt::Unchecked
        )),
        (QList<int>{1, 3, 6, 8})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "a",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{1})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "th",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            -1,
            Qt::PartiallyChecked,
            Qt::PartiallyChecked
        )),
        (QList<int>{5, 6, 7, 8, 9})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            1,
            Qt::Checked,
            Qt::Checked
        )),
        (QList<int>{5})
    )

    // TEST
    TEST_COMPARE(
        toIds(db::theme::reads(
            "t",
            1,
            Qt::PartiallyChecked,
            Qt::Checked
        )),
        (QList<int>{5})
    )

    // TEST
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
}
