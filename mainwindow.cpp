#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>
#include <QTime>
#include <QMessageBox>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <memory>
#include <array>
#include <string>

float getCPUTemperature() {
    std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
    if (!tempFile.is_open()) {
        std::cerr << "Не удалось открыть файл температуры процессора." << std::endl;
        return -1.;
    }

    float temp;
    tempFile >> temp;
    tempFile.close();
    return temp / 1000.0;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        std::cerr << "Не удалось открыть процесс." << std::endl;
        return "-1";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

float getGPUTemperature() {
    std::string output = exec("nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader");
    return std::stof(output);
}


void MainWindow::update() {
    float CPUtemp = getCPUTemperature();
    float GPUtemp = getGPUTemperature();

    QString CPUtemperature = QString::fromStdString("CPU: " + std::to_string((int) CPUtemp));
    QString GPUtemperature = QString::fromStdString("GPU: " + std::to_string((int) GPUtemp));

    ui->CPUlabel->setText(CPUtemperature);
    ui->GPUlabel->setText(GPUtemperature);

    if (CPUtemp > 90 && !CPUshown) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Критическое уведомление");
        message.setInformativeText("Температура процессора выше 90 градусов");

        message.move(800, 500);
        message.exec();

        CPUshown = true;

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::setCPUshownFalse);
        timer->start(10000);
    }

    if (GPUtemp > 85 && !GPUshown) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Критическое уведомление");
        message.setInformativeText("Температура видеокарты выше 85 градусов");

        message.move(800, 500);
        message.exec();

        GPUshown = true;

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::setGPUshownFalse);
        timer->start(10000);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}



