#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QThread>
#include <qcheckbox.h>

class QCustomPlot;
class MainWindow;

typedef const size_t (*SortFunction)(std::vector<int>&);

class TestThread : public QThread
{
    Q_OBJECT
    public:
        TestThread(const MainWindow *window, QObject *parent = nullptr);

        struct testResult {
            QVector<double> data;
            QVector<double> time;
            QString name;
        };
        struct testResults {
            QVector<double> keys;
            QVector<testResult> data;
        };
        struct algorithm {
            QString name;
            SortFunction func;
        };

    signals:
        void started();
        void finished(testResults results);

        void progressValChanged(const int &val);
        void progressTextChanged(const QString &text);
        void progressMaxValChanged(const int &val);

    protected:
        void run() override;
    private:
        testResult test(const QVector<double> &keys, const algorithm &alg, int &progress);

        const MainWindow *m_window;
};

class MainWindow : public QWidget
{
    Q_OBJECT
    public:
        MainWindow();

    public slots:
        void test();
        void testEnd(TestThread::testResults results);
        void validateInt(const QString &text);
    private:
        friend TestThread;

        void setControlsEnabled(const bool &val);

        QCustomPlot *m_SwapsPlot = nullptr;
        QCustomPlot *m_TimesPlot = nullptr;

        QHBoxLayout *m_hlayout;
        QVBoxLayout *m_vlayout;
        QHBoxLayout *m_inputsLayout;
        QVBoxLayout *m_leftLayout;
        QVBoxLayout *m_rightLayout;
        QPushButton *m_testButton;
        QLineEdit *m_iterationsCount;
        QLineEdit *m_startIt;
        QLineEdit *m_endIt;
        QLineEdit *m_stepIt;
        QLineEdit *m_min;
        QLineEdit *m_max;
        QProgressBar *m_progressBar;

        TestThread *m_testThread;

        QMap<QString, QCheckBox*> m_checkboxes;
};
