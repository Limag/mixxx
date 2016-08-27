#include <QtCore>
#include <QtGui>

#include "controllers/keyboard/layoututils.h"
#include "controllers/keyboard/layouts.h"

namespace layoutUtils {
    const int kLayoutLen = 48;

    const unsigned char kKeyboardScancodes[kLayoutLen] = {
            0x29, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,  // Digits row
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,        // Upper row
            0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2b,        // Home row
            0x5e, 0x0c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35               // Lower row

            // Note: Although the <LSGT> key is not defined in the scancode set 2,
            // it is added for pc105 compatibility. It's given scancode 0x5e (94)
    };

    QRegExp const keysequenceSeparator("\\+(?!$)");

    unsigned char layoutIndexToScancode(const unsigned char layoutIndex) {
        return kKeyboardScancodes[layoutIndex];
    };

    unsigned char scancodeToLayoutIndex(const unsigned char scancode) {
        for (unsigned char i = 0; i < kLayoutLen; i++) {
            if (kKeyboardScancodes[i] == scancode) {
                return i;
            }
        }
        return (unsigned char) -1;
    }

    const KbdKeyChar* getKbdKeyChar(KeyboardLayoutPointer pLayout,
                                    unsigned char scancode,
                                    Qt::KeyboardModifier modifier) {

        // Keyboard layouts only support no modifier and shift modifier
        if (modifier > Qt::ShiftModifier) {
            return {};
        }

        unsigned char layoutIndex = scancodeToLayoutIndex(scancode);
        const KbdKeyChar* pKeyChar = pLayout[layoutIndex];

        if (modifier & Qt::ShiftModifier) {
            pKeyChar++;
        }

        return pKeyChar;
    }

    QList<int> findScancodesForCharacter(KeyboardLayoutPointer pLayout,
                                         const QChar& character,
                                         Qt::KeyboardModifier modifier) {

        // Keyboard layouts only support no modifier and shift modifier
        if (modifier > Qt::ShiftModifier) {
            return {};
        }

        // Get modifier offset (0 for no modifier, 1 for shift modifier)
        int modIndex = modifier & Qt::ShiftModifier ? 1 : 0;

        // Find scancodes that match the given character and modifier in pLayout
        QList<int> scancodes;
        for (int i = 0; i < kLayoutLen; i++) {
            const KbdKeyChar& kbdKeyChar = pLayout[i][modIndex];
            QChar currentCharacter = kbdKeyChar.character;

            if (currentCharacter == character) {
                int scancode = layoutUtils::layoutIndexToScancode((const unsigned char) i);
                scancodes += scancode;
            }
        }

        return scancodes;
    }

    QString keyseqGetKey(const QString &keyseq) {
        return keyseq.split(keysequenceSeparator).last();
    }

    QStringList keyseqGetModifiers(const QString &keyseq) {
        QStringList splitKeyseq = keyseq.split(keysequenceSeparator);
        splitKeyseq.pop_back();
        return splitKeyseq;
    }
}