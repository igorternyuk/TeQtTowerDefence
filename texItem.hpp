#ifndef SCORE_H
#define SCORE_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QFont>
#include <QString>
#include <QColor>
#include <QDebug>

template <typename T>
class TextItem : public QGraphicsTextItem
{
public:
    explicit TextItem(const QString &name, T initValue, double x,
                      double y, const QFont& font, QColor color = Qt::blue,
                      QGraphicsItem *parent = nullptr):
        QGraphicsTextItem(parent), mName{name}, mParameter{initValue},
        mInitValue{initValue}, mColor{color}
    {
        setPos(x,y);
        setPlainText(mName + QString(": ") + QString::number(initValue));
        setDefaultTextColor(mColor);
        setFont(font);
    }

    void set(T val)
    {
        mParameter = val;
        setPlainText(mName + QString(": ") + QString::number(mParameter));
    }

    void increase(T step)
    {
        mParameter += step;
        setPlainText(mName + QString(": ") + QString::number(mParameter));
    }

    void decrease(T step)
    {
        mParameter -= step;
        setPlainText(mName + QString(": ") + QString::number(mParameter));
    }

    void reset()
    {
        mParameter = mInitValue;
    }

    inline auto getName() const noexcept { return mName; }
    inline auto getValue() const noexcept { return mParameter; }
    inline const QColor& getColor() const noexcept { return mColor; }
private:
    QString mName;
    T mParameter, mInitValue;
    QColor mColor;
};

#endif // SCORE_H
