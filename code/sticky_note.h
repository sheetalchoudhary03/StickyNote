#ifndef STICKY_NOTE_H
#define STICKY_NOTE_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDir>

class StickyNote : public QWidget {
    Q_OBJECT

public:
    StickyNote(QWidget *parent = nullptr);
    ~StickyNote();

private slots:
    void saveNote();
    void clearNote();
    void toggleColor();
    void togglePin(bool checked);
    void updateStatus();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QTextEdit *textEdit;
    QLabel *statusLabel;
    QString savePath;
    QString color1 = "#FFF89B";  // pale yellow
    QString color2 = "#BEE3F8";  // pale blue
    int maxChars = 500;
};

#endif // STICKY_NOTE_H
