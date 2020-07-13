#ifndef CTRPLUGINFRAMEWORK_SYSTEM_HID_HIDAPI_HPP
#define CTRPLUGINFRAMEWORK_SYSTEM_HID_HIDAPI_HPP

#include <3ds.h>

namespace CTRPluginFramework
{
    namespace Hid
    {
        /* Research

            // CepdThreadFunc
            E59F0024 E92D4008 E5901024 E2800014 E58D1000 E890000F

            // ExtraPadLifoRing::ReadData (mhgen)
            E92D4FF0 E59DA028 E59DB024 E59AC000 E35C0008

            // ExtraPadReader::ReadLatest (mhgen - xenoblade)
            E92D4070 E1A05000 E24DD028 E3E00000 E1A04001 E1A01000

            // PadReader::ReadLatest (Pokemon ORAS - mhgen - mk7 - chronus arc)
            E92D4070 E1A05000 E24DD018 E3E00000 E1A04001

            // TouchReader::ReadLatest (Pokemon ORAS - mhgen - mk7 - chronus arc)
            E92D4010 E24DD018 E1A04001 E3E01000 E28D3010 E1A02001 E8830006

            // PadReader::ReadLatest (Zelda OOT)
            E92D4070 E24DD018 E1A04000 E3E00000 E58D0010 E58D0014

            // TouchReader::ReadLatest (Zelda OOT)
            E92D4010 E24DD018 E1A04001 E3E01000 E58D1010 E58D1014

            // Jump to GatherStartAndSelect
            EB0051B2 E28DD018 E3A00001 E8BD8070

            // PadReader::GatherStartAndSelect (mhgen - pokemon sun 1.2)
            E2802008 E2801004

            // PadReader::GatherStartAndSelecct (Zelda OOT)
            E8900006 E3A0300C E1D3C002

            // ExtraPadReader::GatherStartAndSelect (mhgen - xenoblade)
            // E2802010 E280100C E2800008

        */
        struct AnalogStickStatus
        {
            s16                 x;
            s16                 y;
        } PACKED;

        struct PadStatus
        {
            u32                 hold;
            u32                 trigger;
            u32                 release;
            AnalogStickStatus   stick;
        } PACKED;

        struct ExtraPadStatus
        {
            AnalogStickStatus   stick;
            AnalogStickStatus   extraStick;
            u32                 hold;
            u32                 trigger;
            u32                 release;
            u8                  batteryLevel;
            bool                isConnected;
            u8                  padding[2];
        } PACKED;

        struct TouchPanelStatus
        {
            u16                 x;
            u16                 y;
            u8                  touch;
            u8                  padding[3];

            static const TouchPanelStatus& Get();
        } PACKED;

        struct IAnalogStickStatus
        {
            s16                 x;
            s16                 y;
        } PACKED;

        struct IExtraPadStatus
        {
            u32                 hold;
            u32                 trigger;
            u32                 release;
            u8                  voltage;
            u8                  isConnected;
            u8                  padding[2];
            IAnalogStickStatus  stick;
            IAnalogStickStatus  extraStick;
        } PACKED;

        struct IPadStatus
        {
            u32                 hold;
            u32                 trigger;
            u32                 release;
            IAnalogStickStatus  stick;
        } PACKED;

        struct LifoRing
        {
            s64                 tickWriteZero;
            s64                 oldTickWriteZero;
            s32                 writePointer;
            u8                  padding[4];
        } PACKED;

        struct PadLifoRing
        {
            s64                 tickWriteZero;
            s64                 oldTickWriteZero;
            s32                 writePointer;
            u8                  padding[4];
            float               Svr2Volume;
            u32                 RawButtons;
            IAnalogStickStatus  RawAnalogStick;
            u8                  padding2[4];
            IPadStatus          buffers[8];
        } PACKED;

        struct ExtraPadLifoRing
        {
            s64                 tickWriteZero;
            s64                 oldTickWriteZero;
            s32                 writePointer;
            u8                  padding[4];
            IExtraPadStatus     RawData;
            IExtraPadStatus     buffers[8];
        } PACKED;

        struct Pad
        {
            u32                 handle;
            PadLifoRing *       pLifoRing;
        } PACKED;

        struct TouchPanel
        {
            u32                 handle;
            u32                 pLifoRing;
        } PACKED;

        struct AnalogStickClamper
        {
            s16                 minOfStickClampCircle;
            s16                 minOfStickClampCross;
            s16                 minOfStickClampMinimum;
            s16                 maxOfStickClampCircle;
            s16                 maxOfStickClampCross;
            s16                 maxOfStickClampMinimum;
            u8                  stickClampMode;
            u8                  padding;
            s16                 threshold;
            float               scale;
            float               stroke;
            float               strokeVelocity;
            float               lastLength;
            float               lastDiff;
        } PACKED;

        struct PadReader
        {
            Pad *               pad;
            s32                 indexOfRead;
            float               latestHold;
            AnalogStickClamper  stickClamper;
            bool                isReadLatestFirst;
            u8                  padding[3];
            u8                  padding2[4];
            s64                 tickOfRead;

            static bool     ReadLatest(void);
            static bool     InstallHooks(void);
        };

        struct ExtraPadReader
        {
            u32                 vtable;
            u32                 padding;

            AnalogStickClamper  extraStickClamper;
            AnalogStickClamper  stickClamper;
            s32                 indexOfRead;
            u32                 latestHold;
            PadReader           padReader;
            bool                isReadLatestFirst;
            u8                  padding2[3];
            u8                  padding3[4];
            s64                 tickOfRead;

            static bool     InstallHooks(void);
        } PACKED;

        struct TouchPanelReader
        {
            TouchPanel *        touchPanel;
            s32                 indexOfRead;
            s64                 tickOfRead;

            static bool     ReadLatest(void);
            static bool     InstallHooks(void);
        } PACKED;

        void    Initialize(void);

        namespace RightStick
        {
            void    Initialize(void);
            void    ReadLatest(PadStatus* padStatus);
        }

        /*

            // TODO: remove if prove to be useless
            struct Pad {
                u32 m_Handle;
                u32 m_pResource;
            };

            struct TouchPanel {
                u32 m_Handle;
                u32 m_pResource;
            };

            struct Accelerometer {
                u32 m_Handle;
                u32 m_pResource;
            };

            struct Gyroscope {
                u32 vtable;
                u32 m_Handle;
                u32 m_pResource;
            };

            struct DebugPad {
                u32 m_Handle;
                u32 m_pResource;
            };

            struct SharedMemoryBlock {
                SharedMemoryBlock *head;
                SharedMemoryBlock *tail;

                u32    m_Addr;
                u32  m_Size;
                bool    m_ReadOnly;
                u8      padding[3];
                u32     handle;
                bool    m_SpaceAllocated;
                u8 padding2[3];
                u32 padding3;
            } ;

            struct HidDevices
            {
                Pad                         pad;
                TouchPanel                  touchPanel;
                Accelerometer               accelerometer;
                Gyroscope                   gyroscope;
                DebugPad                    debugPad;
                u32                         sharedMemoryBlock;
            };

        */


    }
}

#endif
