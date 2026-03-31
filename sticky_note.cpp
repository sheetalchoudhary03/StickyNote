#include "sticky_note.h"
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileInfo>

StickyNote::StickyNote(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Sticky Note");
    setFixedSize(320, 220);

    savePath = QDir::homePath() + "/.sticky_note.txt";

    // Widgets
    textEdit = new QTextEdit(this);
    textEdit->setStyleSheet("background-color: " + color1 + ";");
    textEdit->setPlaceholderText("Type here...");
    connect(textEdit, &QTextEdit::textChanged, this, &StickyNote::updateStatus);

    QPushButton *saveBtn = new QPushButton("Save", this);
    connect(saveBtn, &QPushButton::clicked, this, &StickyNote::saveNote);

    QPushButton *clearBtn = new QPushButton("Clear", this);
    connect(clearBtn, &QPushButton::clicked, this, &StickyNote::clearNote);

    QPushButton *colorBtn = new QPushButton("Color", this);
    connect(colorBtn, &QPushButton::clicked, this, &StickyNote::toggleColor);

    QCheckBox *pinCheck = new QCheckBox("Pin", this);
    connect(pinCheck, &QCheckBox::toggled, this, &StickyNote::togglePin);

    statusLabel = new QLabel("0/500", this);

    // Layouts
    QHBoxLayout *toolbar = new QHBoxLayout;
    toolbar->addWidget(saveBtn);
    toolbar->addWidget(clearBtn);
    toolbar->addWidget(colorBtn);
    toolbar->addStretch();
    toolbar->addWidget(pinCheck);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(toolbar);
    layout->addWidget(textEdit);
    layout->addWidget(statusLabel);

    setLayout(layout);

    // Load previous note
    QFile file(savePath);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        textEdit->setPlainText(content.left(maxChars));
        file.close();
    }
    updateStatus();
}

StickyNote::~StickyNote() {}

void StickyNote::saveNote() {
    QFile file(savePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
        statusLabel->setText("Saved ✔");
    }
}

void StickyNote::clearNote() {
    if (QMessageBox::question(this, "Clear", "Clear all text?") == QMessageBox::Yes) {
        textEdit->clear();
        updateStatus();
    }
}

void StickyNote::toggleColor() {
    QString current = textEdit->palette().color(QPalette::Base).name();
    QString newColor = (current == color1 ? color2 : color1);
    textEdit->setStyleSheet("background-color: " + newColor + ";");
}

void StickyNote::togglePin(bool checked) {
    setWindowFlag(Qt::WindowStaysOnTopHint, checked);
    show();
}

void StickyNote::updateStatus() {
    QString content = textEdit->toPlainText();
    if (content.length() > maxChars) {
        textEdit->setPlainText(content.left(maxChars));
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
    }
    statusLabel->setText(QString::number(textEdit->toPlainText().length()) + "/" + QString::number(maxChars));
}

void StickyNote::closeEvent(QCloseEvent *event) {
    saveNote();
    event->accept();
}
