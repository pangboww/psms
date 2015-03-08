#ifndef INITDB
#define INITDB

#include <QtSql>

QVariant addProduct(QSqlQuery &q, const QString &title, float price, int stock){
    q.addBindValue(title);
    q.addBindValue(price);
    q.addBindValue(stock);
    q.exec();
    return q.lastInsertId();
}

void addTransaction(QSqlQuery &q, int amount, const QDateTime &time, const QVariant &productID){
    q.addBindValue(amount);
    q.addBindValue(time);
    q.addBindValue(productID);
    q.exec();
}

float randomFloat(float a, float b) {
    if (b <= a) return 0;
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

int randomInt(int a, int b){
    if (b <= a) return 0;
    int random = rand() % (b - a) + a;
    return random;
}



QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("products", Qt::CaseInsensitive)
        && tables.contains("transactions", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    if (!q.exec(QLatin1String("create table products(id integer primary key, title varchar not null, price float not null, stock integer check(stock>=0) default 0)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table transactions(id integer primary key, amount integer, time datetime, id_product integer, foreign key(id_product) references products(id))")))
        return q.lastError();

    QList<QString> products;
    products << "Apple" << "Apricot" << "Aubergine" << "Avocado" << "Banana" << "Broccoli" << "Carrot";
    products << "Cherry" << "Clementine" << "Courgette" << "Endive" << "Garlic" << "Grape" << "Wiki fruit";
    products << "Lemon" << "Mango" << "Melon" << "Mushroom" << "Nut" << "Olive" << "Orange" << "Pea";
    products << "Peanut" << "Pear" << "Pepper" << "Pumpkin" << "Radish" << "Raisin" << "Strawberry";
    products << "Tomato" << "Watermelon" << "Zucchini";

    foreach (QString product, products){
        if (!q.prepare(QLatin1String("insert into products(title, price, stock) values(?, ?, ?)")))
            return q.lastError();
        QVariant productID = addProduct(q, product, randomFloat(0, 10.0), randomInt(0, 500));

        QList<QDateTime> transactTime;
        for(int i = 0; i < 100; i++){
            QTime t(randomInt(9,18), randomInt(0,60), randomInt(0,60));
            QDate d(randomInt(2010,2015), randomInt(1,13), randomInt(1,29));
            QDateTime dt(d, t);
            transactTime << dt;
        }
        qSort(transactTime);
        if (!q.prepare(QLatin1String("insert into transactions(amount, time, id_product) values(?, ?, ?)")))
            return q.lastError();
        for(int i = 0; i < 100; i++){
            addTransaction(q, randomInt(1,10), transactTime[i], productID);
        }
    }
    return QSqlError();
}


#endif // INITDB



