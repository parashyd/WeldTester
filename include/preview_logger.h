#ifndef PREVIEW_LOGGER_H
#define PREVIEW_LOGGER_H

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <QString>
#include <QVector>
#include <QByteArray>
#include <QDebug>

static constexpr int PREVIEW_HEADER_SIZE = 412;
static constexpr int PREVIEW_SAMPLE_SIZE = 1;
/*
 * ================================================================
 *  .wtp / .wtpf FILE FORMAT
 *
 *  One Preview frame only.
 *
 *  Header fields are fixed-width character fields.
 *  All header bytes are written exactly as specified below.
 *  Short fields are padded with spaces.
 *  Long fields are truncated.
 *
 *  Header size = 412 bytes
 *
 *  After the header:
 *      Y0 (double)
 *      Y1 (double)
 *      ...
 *      YN (double)
 *
 *  One plotted point = 16 bytes.
 *
 *  .wtp  = normal preview
 *  .wtpf = frozen preview
 *
 *  NOTE:
 *  "Class / PrevClass" is treated as ONE 15-byte field because
 *  the supplied specification allocates 15 bytes to that entry.
 * ================================================================
 */

/*
 * ---------------------- Header layout ----------------------------
 *
 * Offset   Size   Field
 * ---------------------------------------------------------------
 * 0        50     O/P Name
 * 50       15     Model & Unit No
 * 65       10     Division
 * 75       15     Section
 * 90       20     Date & Time
 * 110      10     Line
 * 120       5     Rail
 * 125      10     Test Type
 * 135       2     Mode
 *
 * 137       4     KM
 * 141       8     M
 * 149      10     TP
 * 159       8     Loc
 * 167      15     Stations
 * 182      20     Roll Mark
 * 202       3     Weld
 * 205      10     Probe
 * 215       2     Calset
 * 217       1     CH
 *
 * 218       3     PHG1
 * 221       3     PHG2
 * 224       3     Prev PH
 * 227       3     BPG1
 * 230       3     BPG2
 * 233       3     Prev BP
 * 236       3     DG1
 * 239       3     DG2
 * 242       3     SDG1
 * 245       3     SDG2
 *
 * 248       3     Gain
 * 251       3     Dely
 * 254       4     Range
 * 258       2     RJ
 * 260      15     HM
 * 275      15     Class / PrevClass
 * 290       2     G1ST
 * 292       2     G1ED
 * 294       2     TH1
 * 296       2     G2ST
 * 298       2     G2ED
 * 300       2     TH2
 * 302       5     Jog Plate
 * 307       5     Angle
 * 312     100     Remarks
 *
 * 0 - 411           Header
 * 412                Waveform starts
 * ---------------------------------------------------------------
 */

struct PreviewHeader
{
    QString opName;
    QString modelUnit;
    QString division;
    QString section;
    QString dateTime;
    QString line;
    QString rail;
    QString testType;
    QString mode;

    QString km;
    QString m;
    QString tp;
    QString loc;
    QString stations;
    QString rollMark;
    QString weld;
    QString probe;
    QString calset;
    QString ch;

    QString phg1;
    QString phg2;
    QString prevPH;
    QString bpg1;
    QString bpg2;
    QString prevBP;
    QString dg1;
    QString dg2;
    QString sdg1;
    QString sdg2;

    QString gain;
    QString dely;
    QString range;
    QString rj;
    QString hm;
    QString classPrevClass;
    QString g1st;
    QString g1ed;
    QString th1;
    QString g2st;
    QString g2ed;
    QString th2;
    QString jogPlate;
    QString angle;
    QString remarks;
};

namespace preview_detail
{
    inline QByteArray fixedField(const QString &value, int size)
    {
        QByteArray data = value.toLocal8Bit();

        if (data.size() > size)
            data.truncate(size);

        if (data.size() < size)
            data.append(QByteArray(size - data.size(), ' '));

        return data;
    }

    inline void putField(uint8_t *buffer,
                         int &offset,
                         const QString &value,
                         int size)
    {
        QByteArray field = fixedField(value, size);
        memcpy(buffer + offset, field.constData(), size);
        offset += size;
    }

    inline void packHeader(uint8_t buffer[PREVIEW_HEADER_SIZE],
                           const PreviewHeader &h)
    {
        memset(buffer, ' ', PREVIEW_HEADER_SIZE);

        int offset = 0;

        putField(buffer, offset, h.opName,        50);
        putField(buffer, offset, h.modelUnit,     15);
        putField(buffer, offset, h.division,      10);
        putField(buffer, offset, h.section,       15);
        putField(buffer, offset, h.dateTime,      20);
        putField(buffer, offset, h.line,          10);
        putField(buffer, offset, h.rail,            5);
        putField(buffer, offset, h.testType,      10);
        putField(buffer, offset, h.mode,            2);

        putField(buffer, offset, h.km,              4);
        putField(buffer, offset, h.m,               8);
        putField(buffer, offset, h.tp,             10);
        putField(buffer, offset, h.loc,              8);
        putField(buffer, offset, h.stations,       15);
        putField(buffer, offset, h.rollMark,       20);
        putField(buffer, offset, h.weld,             3);
        putField(buffer, offset, h.probe,           10);
        putField(buffer, offset, h.calset,           2);
        putField(buffer, offset, h.ch,               1);

        putField(buffer, offset, h.phg1,              3);
        putField(buffer, offset, h.phg2,              3);
        putField(buffer, offset, h.prevPH,            3);
        putField(buffer, offset, h.bpg1,              3);
        putField(buffer, offset, h.bpg2,              3);
        putField(buffer, offset, h.prevBP,            3);
        putField(buffer, offset, h.dg1,               3);
        putField(buffer, offset, h.dg2,               3);
        putField(buffer, offset, h.sdg1,              3);
        putField(buffer, offset, h.sdg2,              3);

        putField(buffer, offset, h.gain,              3);
        putField(buffer, offset, h.dely,              3);
        putField(buffer, offset, h.range,             4);
        putField(buffer, offset, h.rj,                2);
        putField(buffer, offset, h.hm,               15);
        putField(buffer, offset, h.classPrevClass,   15);
        putField(buffer, offset, h.g1st,               2);
        putField(buffer, offset, h.g1ed,               2);
        putField(buffer, offset, h.th1,                2);
        putField(buffer, offset, h.g2st,               2);
        putField(buffer, offset, h.g2ed,               2);
        putField(buffer, offset, h.th2,                2);
        putField(buffer, offset, h.jogPlate,           5);
        putField(buffer, offset, h.angle,              5);
        putField(buffer, offset, h.remarks,          100);

        Q_ASSERT(offset == PREVIEW_HEADER_SIZE);
    }
}

class PreviewLogger
{
public:
    PreviewLogger() = default;

    ~PreviewLogger()
    {
        close();
    }

    bool open(const QString &filePath, const PreviewHeader &header)
    {
        close();

        m_sampleCount = 0;
        m_frameWritten = false;

        m_fp = fopen(filePath.toLocal8Bit().constData(), "wb");

        if (!m_fp)
        {
            qWarning() << "[PreviewLogger] fopen failed:" << filePath;
            return false;
        }

        uint8_t buffer[PREVIEW_HEADER_SIZE];

        preview_detail::packHeader(buffer, header);

        if (fwrite(buffer, 1, PREVIEW_HEADER_SIZE, m_fp)
            != PREVIEW_HEADER_SIZE)
        {
            qWarning() << "[PreviewLogger] Failed to write header:"
                       << filePath;

            fclose(m_fp);
            m_fp = nullptr;
            return false;
        }

        qDebug() << "[PreviewLogger] Recording started ->"
                 << filePath
                 << "headerSize=" << PREVIEW_HEADER_SIZE;

        return true;
    }


    bool writeFrame(const QVector<double> &amplitudes)
    {
        if (!m_fp) return false;

        const int sampleCount = amplitudes.size();

        if (sampleCount <= 0)
            return false;

        QByteArray frame(sampleCount, 0);

        for (int i = 0; i < sampleCount; ++i)
        {
            frame[i] = static_cast<uint8_t>(amplitudes[i]);
        }

        fwrite(frame.constData(), 1, sampleCount, m_fp);

        m_sampleCount = sampleCount;
        m_frameWritten = true;

        return true;
    }

    void close()
    {
        if (!m_fp)
            return;

        fflush(m_fp);
        fclose(m_fp);
        m_fp = nullptr;

        qDebug() << "[PreviewLogger] Recording stopped."
                 << "Samples:" << m_sampleCount
                 << "File size:"
                 << (PREVIEW_HEADER_SIZE
                     + static_cast<long long>(m_sampleCount)
                       * PREVIEW_SAMPLE_SIZE)
                 << "bytes";

        m_sampleCount = 0;
        m_frameWritten = false;
    }

    bool isOpen() const
    {
        return m_fp != nullptr;
    }

    bool frameWritten() const
    {
        return m_frameWritten;
    }

    int sampleCount() const
    {
        return m_sampleCount;
    }

    long long waveformSize() const
    {
        return static_cast<long long>(m_sampleCount)
               * PREVIEW_SAMPLE_SIZE;
    }

    long long fileSize() const
    {
        return PREVIEW_HEADER_SIZE + waveformSize();
    }

private:
    FILE *m_fp = nullptr;
    int m_sampleCount = 0;
    bool m_frameWritten = false;
};

#endif // PREVIEW_LOGGER_H
