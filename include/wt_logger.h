#ifndef WT_LOGGER_H
#define WT_LOGGER_H
#pragma once
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include "DataFile.h"

// ═══════════════════════════════════════════════════════════════
//  .wt  FILE FORMAT  (all values little-endian)
//
//  Offset  Size  Type    Field
//  ──────────────────────────────────────────────────
//  0       4     uint32  Magic: 0x574C5447  ('WLTG')
//  ── ConfigEntry (72 bytes) ─────────────────────────
//  4       4     int32   calset
//  8       4     int32   channel
//  12      4     int32   velocity
//  16      8     double  range
//  24      8     double  delay
//  32      8     double  reject
//  40      4     int32   g1_start
//  44      4     int32   g1_end
//  48      4     int32   th1
//  52      4     int32   g2_start
//  56      4     int32   g2_end
//  60      4     int32   th2
//  64      8     double  Gain
//  72      4     float   Angle
//  ── Playback metadata ──────────────────────────────
//  76      4     int32   totalFrames  (patched on close)
//  80      4     int32   frameSize    (bytes per frame)
//  ── Data ───────────────────────────────────────────
//  84      frameSize × totalFrames    raw amplitude bytes (0–100)
// ═══════════════════════════════════════════════════════════════

static constexpr uint32_t WT_MAGIC        = 0x574C5447u; // 'WLTG'
static constexpr int      WT_HEADER_SIZE  = 84;

//static constexpr float    RANGE_FACTOR_LT30 = 3.378f;
//static constexpr float    RANGE_FACTOR_GT30 = 6.212f;

// ─────────────────────────────────────────────────────────────
//  ConfigEntry is defined in datafile.h
//  Helper free-functions – no modification to the typedef struct
// ─────────────────────────────────────────────────────────────
inline float wtRangeFactor(const ConfigEntry &c)
{
    return c.Angle < 30.f ? RANGE_FACTOR_LT30 : RANGE_FACTOR_GT30;
}

inline int wtFrameSize(const ConfigEntry &c)
{
    return static_cast<int>(c.range * wtRangeFactor(c));
}

// ─────────────────────────────────────────────────────────────
//  Header read / write helpers  (plain memcpy, no QDataStream)
// ─────────────────────────────────────────────────────────────
namespace wt_detail {

// Pack header into a fixed 84-byte buffer
inline void packHeader(uint8_t buf[WT_HEADER_SIZE],
                       const ConfigEntry &c,
                       int32_t totalFrames,
                       int32_t frameSize)
{
    memset(buf, 0, WT_HEADER_SIZE);
    int off = 0;

    // Magic
    memcpy(buf + off, &WT_MAGIC, 4);          off += 4;

    // ConfigEntry fields in order
    int32_t i32;
    double  d64;
    float   f32;

    i32 = c.calset;    memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.channel;   memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.velocity;  memcpy(buf + off, &i32, 4); off += 4;
    d64 = c.range;     memcpy(buf + off, &d64, 8); off += 8;
    d64 = c.delay;     memcpy(buf + off, &d64, 8); off += 8;
    d64 = c.reject;    memcpy(buf + off, &d64, 8); off += 8;
    i32 = c.g1_start;  memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.g1_end;    memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.th1;       memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.g2_start;  memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.g2_end;    memcpy(buf + off, &i32, 4); off += 4;
    i32 = c.th2;       memcpy(buf + off, &i32, 4); off += 4;
    d64 = c.Gain;      memcpy(buf + off, &d64, 8); off += 8;
    f32 = c.Angle;     memcpy(buf + off, &f32, 4); off += 4;  // offset 72

    // Metadata
    memcpy(buf + off, &totalFrames, 4); off += 4;  // offset 76
    memcpy(buf + off, &frameSize,   4);            // offset 80
}

// Unpack header from a fixed 84-byte buffer
inline bool unpackHeader(const uint8_t buf[WT_HEADER_SIZE],
                         ConfigEntry &c,
                         int32_t &totalFrames,
                         int32_t &frameSize)
{
    int off = 0;

    uint32_t magic = 0;
    memcpy(&magic, buf + off, 4); off += 4;
    if (magic != WT_MAGIC)
    {
        qWarning() << "[wt] Bad magic:" << Qt::hex << magic;
        return false;
    }

    int32_t i32;
    double  d64;
    float   f32;

    memcpy(&i32, buf + off, 4); c.calset   = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.channel  = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.velocity = i32; off += 4;
    memcpy(&d64, buf + off, 8); c.range    = d64; off += 8;
    memcpy(&d64, buf + off, 8); c.delay    = d64; off += 8;
    memcpy(&d64, buf + off, 8); c.reject   = d64; off += 8;
    memcpy(&i32, buf + off, 4); c.g1_start = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.g1_end   = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.th1      = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.g2_start = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.g2_end   = i32; off += 4;
    memcpy(&i32, buf + off, 4); c.th2      = i32; off += 4;
    memcpy(&d64, buf + off, 8); c.Gain     = d64; off += 8;
    memcpy(&f32, buf + off, 4); c.Angle    = f32; off += 4;

    memcpy(&totalFrames, buf + off, 4); off += 4;
    memcpy(&frameSize,   buf + off, 4);

    if (frameSize <= 0 || totalFrames < 0)
    {
        qWarning() << "[wt] Invalid frameSize or totalFrames";
        return false;
    }
    return true;
}

} // namespace wt_detail


// ═══════════════════════════════════════════════════════════════
//  WtLogger  —  recording side (used in TestScreen)
// ═══════════════════════════════════════════════════════════════
class WtLogger
{
public:
    WtLogger()  = default;
    ~WtLogger() { close(); }

    // Call when DAC button pressed to START recording
    bool open(const QString &filePath, const ConfigEntry &config)
    {
        close(); // safety

        m_frameSize  = wtFrameSize(config);
        m_frameCount = 0;

        if (m_frameSize <= 0)
        {
            qWarning() << "[WtLogger] Invalid frameSize" << m_frameSize;
            return false;
        }

        m_fp = fopen(filePath.toLocal8Bit().constData(), "wb");
        if (!m_fp)
        {
            qWarning() << "[WtLogger] fopen failed:" << filePath;
            return false;
        }

        // Write header – totalFrames is 0 for now, patched on close()
        uint8_t buf[WT_HEADER_SIZE];
        wt_detail::packHeader(buf, config, 0, m_frameSize);
        fwrite(buf, 1, WT_HEADER_SIZE, m_fp);

        qDebug() << "[WtLogger] Recording started ->" << filePath
                 << "  frameSize=" << m_frameSize;
        return true;
    }

    // Call every 20 ms with amplitude data (values 0–100)
    void writeFrame(const QByteArray &amplitudes)
    {
        if (!m_fp) return;

        // Fixed-size frame – zero padded if short, clipped if long
        uint8_t frame[m_frameSize];
        memset(frame, 0, m_frameSize);
        const int copy = qMin((int)amplitudes.size(), m_frameSize);
        if (copy > 0)
            memcpy(frame, amplitudes.constData(), copy);

        fwrite(frame, 1, m_frameSize, m_fp);
        ++m_frameCount;
    }

    // Call when DAC button pressed to STOP recording
    void close()
    {
        if (!m_fp) return;

        // Patch totalFrames at offset 76
        fseek(m_fp, 76, SEEK_SET);
        int32_t fc = m_frameCount;
        fwrite(&fc, sizeof(int32_t), 1, m_fp);

        fflush(m_fp);
        fclose(m_fp);
        m_fp = nullptr;

        qDebug() << "[WtLogger] Recording stopped."
                 << "Frames:" << m_frameCount
                 << "  File size:"
                 << (WT_HEADER_SIZE + (long long)m_frameCount * m_frameSize)
                 << "bytes";
    }

    bool isOpen()     const { return m_fp != nullptr; }
    int  frameCount() const { return m_frameCount; }
    int  frameSize()  const { return m_frameSize; }

private:
    FILE *m_fp        = nullptr;
    int   m_frameSize  = 0;
    int   m_frameCount = 0;
};


// ═══════════════════════════════════════════════════════════════
//  WtReader  —  playback side (used in ViewLogData)
// ═══════════════════════════════════════════════════════════════
class WtReader
{
public:
    WtReader()  = default;
    ~WtReader() { close(); }

    bool open(const QString &filePath)
    {
        close();

        m_fp = fopen(filePath.toLocal8Bit().constData(), "rb");
        if (!m_fp)
        {
            qWarning() << "[WtReader] fopen failed:" << filePath;
            return false;
        }

        if (!readHeader())
        {
            fclose(m_fp);
            m_fp = nullptr;
            return false;
        }

        m_currentFrame = 0;

        qDebug() << "[WtReader] Opened:" << filePath
                 << "\n  channel="   << m_config.channel
                 << "  calset="      << m_config.calset
                 << "  range="       << m_config.range
                 << "  angle="       << m_config.Angle
                 << "\n  frameSize=" << m_frameSize
                 << "  totalFrames=" << m_totalFrames
                 << "  duration="    << durationSecs() << "s";
        return true;
    }

    void close()
    {
        if (m_fp) { fclose(m_fp); m_fp = nullptr; }
        m_currentFrame = 0;
        m_totalFrames  = 0;
        m_frameSize    = 0;
    }

    // Returns the next frame. Empty = end of file.
    QByteArray readNextFrame()
    {
        if (!m_fp || atEnd()) return {};

        long offset = WT_HEADER_SIZE + (long)m_currentFrame * m_frameSize;
        fseek(m_fp, offset, SEEK_SET);

        QByteArray data(m_frameSize, '\0');
        int bytesRead = (int)fread(data.data(), 1, m_frameSize, m_fp);
        if (bytesRead != m_frameSize)
            data.resize(bytesRead);  // short read at end – shouldn't happen

        ++m_currentFrame;
        return data;
    }

    // Jump to any frame (scrubbing)
    void seekToFrame(int frame)
    {
        m_currentFrame = qBound(0, frame, m_totalFrames - 1);
    }

    bool           isOpen()       const { return m_fp != nullptr; }
    bool           atEnd()        const { return m_currentFrame >= m_totalFrames; }
    int            currentFrame() const { return m_currentFrame; }
    int            totalFrames()  const { return m_totalFrames; }
    int            frameSize()    const { return m_frameSize; }
    double         durationSecs() const { return m_totalFrames * 0.020; }
    float          progress()     const
    {
        return m_totalFrames > 0
                   ? (float)m_currentFrame / m_totalFrames : 0.f;
    }
    const ConfigEntry &config()   const { return m_config; }

private:
    bool readHeader()
    {
        // Get file size to validate
        fseek(m_fp, 0, SEEK_END);
        long fileSize = ftell(m_fp);
        fseek(m_fp, 0, SEEK_SET);

        if (fileSize < WT_HEADER_SIZE)
        {
            qWarning() << "[WtReader] File too small";
            return false;
        }

        uint8_t buf[WT_HEADER_SIZE];
        if (fread(buf, 1, WT_HEADER_SIZE, m_fp) != WT_HEADER_SIZE)
        {
            qWarning() << "[WtReader] Failed to read header";
            return false;
        }

        int32_t totalFrames = 0, frameSize = 0;
        if (!wt_detail::unpackHeader(buf, m_config, totalFrames, frameSize))
            return false;

        m_totalFrames = totalFrames;
        m_frameSize   = frameSize;
        return true;
    }

    FILE       *m_fp           = nullptr;
    ConfigEntry m_config       = {};
    int         m_totalFrames  = 0;
    int         m_frameSize    = 0;
    int         m_currentFrame = 0;
};
#endif // WT_LOGGER_H
