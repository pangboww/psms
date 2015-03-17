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

void addProvider(QSqlQuery &q, const QString &name){
    q.addBindValue(name);
    q.exec();
}

void addSale(QSqlQuery &q, int amount, const QDateTime &time, const QVariant &productID){
    q.addBindValue(amount);
    q.addBindValue(time.toString("yyyy-MM-dd HH:mm:ss"));
    q.addBindValue(productID);
    q.exec();
}

void addPurchase(QSqlQuery &q, int amount, const QDateTime &time, const QVariant &productID, int providerID){
    q.addBindValue(amount);
    q.addBindValue(time.toString("yyyy-MM-dd HH:mm:ss"));
    q.addBindValue(productID);
    q.addBindValue(providerID);
    q.exec();
}

float randomFloat(float a, float b) {
    if (b <= a) return 0;
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    float size = a + r;
    QString str = QString::number(size, 'f', 2);
    return str.toFloat();
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
    if (!q.exec(QLatin1String("create table sales(id integer primary key, amount integer, time text, id_product integer, foreign key(id_product) references products(id))")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table providers(id integer primary key, name varchar not null)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table purchases(id integer primary key, amount integer, time text, id_product integer, id_provider integer, foreign key(id_product) references products(id),foreign key(id_provider) references providers(id))")))
        return q.lastError();


    QList<QString> products;
    products << "Apple" << "Apricot" << "Aubergine" << "Avocado" << "Banana" << "Broccoli" << "Carrot";
    products << "Cherry" << "Clementine" << "Courgette" << "Endive" << "Garlic" << "Grape" << "Wiki fruit";
    products << "Lemon" << "Mango" << "Melon" << "Mushroom" << "Nut" << "Olive" << "Orange" << "Pea";
    products << "Peanut" << "Pear" << "Pepper" << "Pumpkin" << "Radish" << "Raisin" << "Strawberry";
    products << "Tomato" << "Watermelon" << "Zucchini";

    QList<QString> providers;
    providers << "Lucas" << "Nathan" << "Enzo" << "Léo" << "Gabriel" << "Louis" << "Hugo" << "Raphaël";
    providers << "Emma"	<< "Léa" << "Chloé" << "Inès" << "Manon" << "Jade" << "Lola" << "Camille";

    foreach(QString provider, providers){
        if (!q.prepare(QLatin1String("insert into providers(name) values(?)")))
            return q.lastError();
        addProvider(q, provider);
    }

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
        if (!q.prepare(QLatin1String("insert into sales(amount, time, id_product) values(?, ?, ?)")))
            return q.lastError();
        for(int i = 0; i < 100; i++){
            addSale(q, randomInt(1,10), transactTime[i], productID);
        }

        transactTime.clear();
        for(int i = 0; i < 10; i++){
            QTime t(randomInt(9,18), randomInt(0,60), randomInt(0,60));
            QDate d(randomInt(2010,2015), randomInt(1,13), randomInt(1,29));
            QDateTime dt(d, t);
            transactTime << dt;
        }
        qSort(transactTime);
        if (!q.prepare(QLatin1String("insert into purchases(amount, time, id_product, id_provider) values(?, ?, ?, ?)")))
            return q.lastError();
        for(int i = 0; i < 10; i++){
            addPurchase(q, randomInt(100,200), transactTime[i], productID, randomInt(1, providers.size()));
        }
    }



    return QSqlError();
}


#endif // INITDB



