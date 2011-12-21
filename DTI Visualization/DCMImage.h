#ifndef DICOMIMAGE_H
#define DICOMIMAGE_H

// Program to decode a DICOM image.
// Written by Amarnath S, Mahesh Reddy S, Bangalore, India, April 2009.
// Updated by Harsha T, Apr 2010.

// Inspired heavily by ImageJ

class DicomImage
{
private:
    const uint PIXEL_REPRESENTATION       = 0x00280103;
    const uint TRANSFER_SYNTAX_UID        = 0x00020010;
    const uint SLICE_THICKNESS            = 0x00180050;
    const uint SLICE_SPACING              = 0x00180088;
    const uint SAMPLES_PER_PIXEL          = 0x00280002;
    const uint PHOTOMETRIC_INTERPRETATION = 0x00280004;
    const uint PLANAR_CONFIGURATION       = 0x00280006;
    const uint NUMBER_OF_FRAMES           = 0x00280008;
    const uint ROWS                       = 0x00280010;
    const uint COLUMNS                    = 0x00280011;
    const uint PIXEL_SPACING              = 0x00280030;
    const uint BITS_ALLOCATED             = 0x00280100;
    const uint WINDOW_CENTER              = 0x00281050;
    const uint WINDOW_WIDTH               = 0x00281051;
    const uint RESCALE_INTERCEPT          = 0x00281052;
    const uint RESCALE_SLOPE              = 0x00281053;
    const uint RED_PALETTE                = 0x00281201;
    const uint GREEN_PALETTE              = 0x00281202;
    const uint BLUE_PALETTE               = 0x00281203;
    const uint ICON_IMAGE_SEQUENCE        = 0x00880200;
    const uint PIXEL_DATA                 = 0x7FE00010;

    const string ITEM                     = "FFFEE000";
    const string ITEM_DELIMITATION        = "FFFEE00D";
    const string SEQUENCE_DELIMITATION    = "FFFEE0DD";

    const int
        AE = 0x4145,
        AS = 0x4153,
        AT = 0x4154,
        CS = 0x4353,
        DA = 0x4441,
        DS = 0x4453,
        DT = 0x4454,
        FD = 0x4644,
        FL = 0x464C,
        IS = 0x4953,
        LO = 0x4C4F,
        LT = 0x4C54,
        PN = 0x504E,
        SH = 0x5348,
        SL = 0x534C,
        SS = 0x5353,
        ST = 0x5354,
        TM = 0x544D,
        UI = 0x5549,
        UL = 0x554C,
        US = 0x5553,
        UT = 0x5554,
        OB = 0x4F42,
        OW = 0x4F57,
        SQ = 0x5351,
        UN = 0x554E,
        QQ = 0x3F3F;

        const int ID_OFFSET = 128;  //location of "DICM"
        const String DICM   = "DICM";
        const int IMPLICIT_VR = 0x2D2D; // '--' 

};
/*
namespace DicomImageViewer
{
    class DicomDecoder
    {

        DicomDictionary dic;

        String dicomFileName;

        BinaryReader file;
        int location = 0;
        bool littleEndian = true;

        int elementLength;
        int vr;  // Value Representation
        byte[] vrLetters      = new byte[2];
        public List<string> dicomInfo;
        public bool dicmFound; // "DICM" found at offset 128
        bool oddLocations;  // one or more tags at odd locations
        bool bigEndianTransferSyntax = false;
        double rescaleIntercept, rescaleSlope;
        bool inSequence;
        
        // Amarnath's fields
        public int bitsAllocated;
        public int width;
        public int height;
        public int offset;
        public int nImages;
        public double pixelDepth = 1.0;
        public double pixelWidth = 1.0;
        public double pixelHeight = 1.0;
        public string unit;
        byte[] reds;
        byte[] greens;
        byte[] blues;
        List<byte> pixels8;
        List<ushort> pixels16;
        public bool dicomFileReadSuccess;
        public bool compressedImage; // True if the image data is compressed, false otherwise.
        public bool dicomDir;
        public double windowCentre, windowWidth;

        public DicomDecoder()
        {
            dic = new DicomDictionary();
            dicomFileReadSuccess = false;
            dicomInfo = new List<string>();
        }

        public string DicomFileName
        {
            set
            {
                dicomFileName = value;
                dicomFileReadSuccess = false;
                windowCentre = 0;
                windowWidth = 0;
                file = new BinaryReader(File.Open(dicomFileName, FileMode.Open));
                location = 0; // Reset the location
                dicomDir = false;
                dicomInfo.Clear();
                try
                {
                    bool readResult = ReadFileInfo();
                    if (readResult == true)
                    {
                        ReadPixels();
                        dicomFileReadSuccess = true;
                    }
                    else
                    {
                        dicomFileReadSuccess = false;
                    }
                }
                catch 
                {
                    dicomDir = true;
                }
                finally
                {
                    file.Close();
                }
            }
        }

        public void GetPixels8(ref List<byte> pixels)
        {
            pixels = pixels8;
        }

        public void GetPixels16(ref List<ushort> pixels)
        {
            pixels = pixels16;
        }

        String GetString(int length)
        {
            byte[] buf = new byte[length];
            file.BaseStream.Position = location;
            int count = file.Read(buf, 0, length);
            location += length;
            string s = System.Text.ASCIIEncoding.ASCII.GetString(buf);
            return s;
        }

        byte GetByte() // Changed return type to byte
        {
            file.BaseStream.Position = location;
            byte b = file.ReadByte();
            ++location;
            return b;
        }

        ushort GetShort() // Changed return type to short
        {
            byte b0 = GetByte();
            byte b1 = GetByte();
            ushort s;
            if (littleEndian)
                s = Convert.ToUInt16((b1 << 8) + b0);
            else
                s = Convert.ToUInt16((b0 << 8) + b1);
            return s;
        }

        int GetInt()
        {
            byte b0 = GetByte();
            byte b1 = GetByte();
            byte b2 = GetByte();
            byte b3 = GetByte();
            int i;
            if (littleEndian)
                i = (b3 << 24) + (b2 << 16) + (b1 << 8) + b0;
            else
                i = (b0 << 24) + (b1 << 16) + (b2 << 8) + b3;
            return i;
        }

        double GetDouble()
        {
            byte b0 = GetByte();
            byte b1 = GetByte();
            byte b2 = GetByte();
            byte b3 = GetByte();
            byte b4 = GetByte();
            byte b5 = GetByte();
            byte b6 = GetByte();
            byte b7 = GetByte();

            long res = 0;
            if (littleEndian)
            {
                res += b0;
                res += (((long)b1) << 8);
                res += (((long)b2) << 16);
                res += (((long)b3) << 24);
                res += (((long)b4) << 32);
                res += (((long)b5) << 40);
                res += (((long)b6) << 48);
                res += (((long)b7) << 56);
            }
            else
            {
                res += b7;
                res += (((long)b6) << 8);
                res += (((long)b5) << 16);
                res += (((long)b4) << 24);
                res += (((long)b3) << 32);
                res += (((long)b2) << 40);
                res += (((long)b1) << 48);
                res += (((long)b0) << 56);
            }

            double d = Convert.ToDouble(res, new CultureInfo("en-US"));
            return d;
        }

        float GetFloat()
        {
            byte b0 = GetByte();
            byte b1 = GetByte();
            byte b2 = GetByte();
            byte b3 = GetByte();

            int res = 0;

            if (littleEndian)
            {
                res += b0;
                res += (((int)b1) << 8);
                res += (((int)b2) << 16);
                res += (((int)b3) << 24);
            }
            else
            {
                res += b3;
                res += (((int)b2) << 8);
                res += (((int)b1) << 16);
                res += (((int)b0) << 24);
            }

            float f1;
            f1 = Convert.ToSingle(res, new CultureInfo("en-US"));
            return f1;
        }

        byte[] GetLut(int length)
        {
            if ((length & 1) != 0) // odd
            {
                String dummy = GetString(length);
                return null;
            }

            length /= 2;
            byte[] lut = new byte[length];
            for (int i = 0; i < length; ++i)
                lut[i] = Convert.ToByte(GetShort() >> 8);
            return lut;
        }

        int GetLength()
        {
            byte b0 = GetByte();
            byte b1 = GetByte();
            byte b2 = GetByte();
            byte b3 = GetByte();

            // Cannot know whether the VR is implicit or explicit without the 
            // complete Dicom Data Dictionary. 
            vr = (b0 << 8) + b1;

            switch (vr)
            {
                case OB:
                case OW:
                case SQ:
                case UN:
                case UT:
                    // Explicit VR with 32-bit length if other two bytes are zero
                    if ((b2 == 0) || (b3 == 0)) return GetInt();
                    // Implicit VR with 32-bit length
                    vr = IMPLICIT_VR;
                    if (littleEndian)
                        return ((b3 << 24) + (b2 << 16) + (b1 << 8) + b0);
                    else
                        return ((b0 << 24) + (b1 << 16) + (b2 << 8) + b3);
                    // break; // Not necessary
                case AE:
                case AS:
                case AT:
                case CS:
                case DA:
                case DS:
                case DT:
                case FD:
                case FL:
                case IS:
                case LO:
                case LT:
                case PN:
                case SH:
                case SL:
                case SS:
                case ST:
                case TM:
                case UI:
                case UL:
                case US:
                case QQ:
                    // Explicit vr with 16-bit length
                    if (littleEndian)
                        return ((b3 << 8) + b2);
                    else
                        return ((b2 << 8) + b3);
                default:
                    // Implicit VR with 32-bit length...
                    vr = IMPLICIT_VR;
                    if (littleEndian)
                        return ((b3 << 24) + (b2 << 16) + (b1 << 8) + b0);
                    else
                        return ((b0 << 24) + (b1 << 16) + (b2 << 8) + b3);
            }
        }

        int GetNextTag()
        {
            int groupWord = GetShort();
            if (groupWord == 0x0800 && bigEndianTransferSyntax)
            {
                littleEndian = false;
                groupWord = 0x0008;
            }
            int elementWord = GetShort();
            int tag = groupWord << 16 | elementWord;
            
            elementLength = GetLength();

            // Hack to read some GE files
            if (elementLength == 13 && !oddLocations)
                elementLength = 10;

            // "Undefined" element length.
            // This is a sort of bracket that encloses a sequence of elements.
            if (elementLength == -1)
            {
                elementLength = 0;
                inSequence = true;
            }
            return tag;
        }

        String GetHeaderInfo(int tag, String value)
        {
            string str = tag.ToString("X8");
            if (str == ITEM_DELIMITATION || str == SEQUENCE_DELIMITATION)
            {
                inSequence = false;
                return null;
            }

            string id = null;
            
            if (dic.dict.ContainsKey(str))
            {
                id = dic.dict[str];
                if (id != null)
                {
                    if (vr == IMPLICIT_VR)
                        vr = (id[0] << 8) + id[1];
                    id = id.Substring(2);
                }
            }

            if (str == ITEM)
                return (id != null ? id : ":null");
            if (value != null)
                return id + ": " + value;

            switch (vr)
            {
                case FD:
                    for (int i = 0; i < elementLength; ++i)
                        GetByte();
                    break;
                case FL:
                    for (int i = 0; i < elementLength; i++)
                        GetByte();
                    break;
                case AE:
                case AS:
                case AT:
                case CS:
                case DA:
                case DS:
                case DT:
                case IS:
                case LO:
                case LT:
                case PN:
                case SH:
                case ST:
                case TM:
                case UI:
                    value = GetString(elementLength);
                    break;
                case US:
                    if (elementLength == 2)
                        value = Convert.ToString(GetShort());
                    else
                    {
                        value = "";
                        int n = elementLength / 2;
                        for (int i = 0; i < n; i++)
                            value += Convert.ToString(GetShort()) + " ";
                    }
                    break;
                case IMPLICIT_VR:
                    value = GetString(elementLength);
                    if (elementLength > 44)
                        value = null;
                    break;
                case SQ:
                    value = "";
                    bool privateTag = ((tag >> 16) & 1) != 0;
                    if (tag != ICON_IMAGE_SEQUENCE && !privateTag)
                        break;
                    goto default;
                default:
                    location += elementLength;
                    value = "";
                    break;
            }

            if (value != null && id == null && value != "")
                return "---: " + value;
            else if (id == null)
                return null;
            else
                return id + ": " + value;
        }

        void AddInfo(int tag, string value)
        {
            string info = GetHeaderInfo(tag, value);

            string str = tag.ToString("X");
            string strPadded = str.PadLeft(8, '0');
            string strInfo;
            if (inSequence && info != null && vr != SQ)
                info = ">" + info;
            if (info != null && str != ITEM)
            {
                if (info.Contains("---"))
                    strInfo = info.Replace("---", "Private Tag");
                else
                    strInfo = info;

                dicomInfo.Add(strPadded + "//" + strInfo);
            }
        }

        void AddInfo(int tag, int value)
        {
            AddInfo(tag, Convert.ToString(value));
        }

        void GetSpatialScale(String scale)
        {
            double xscale = 0, yscale = 0;
            int i = scale.IndexOf('\\');
            if (i > 0)
            {
                yscale = Convert.ToDouble(scale.Substring(0, i), new CultureInfo("en-US"));
                xscale = Convert.ToDouble(scale.Substring(i + 1), new CultureInfo("en-US"));
            }
            if (xscale != 0.0 && yscale != 0.0)
            {
                pixelWidth = xscale;
                pixelHeight = yscale;
                unit = "mm";
            }
        }

        public bool ReadFileInfo()
        {
            long skipCount = Convert.ToInt32(ID_OFFSET);
            bitsAllocated = 16;
            compressedImage = false;

            file.BaseStream.Seek(skipCount, SeekOrigin.Begin);
            location += ID_OFFSET;

            if (GetString(4) != DICM)
            {
                file.Close();
                // Return gracefully indicating that this is not a DICOM file
                dicmFound = false;
                return false;
            }
            else
            {
                dicmFound = true;
            }

            bool decodingTags = true;
            int samplesPerPixel = 1;
            int planarConfiguration = 0;
            String photoInterpretation = "";

            while (decodingTags)
            {
                int tag = GetNextTag();
                if ((location & 1) != 0)
                    oddLocations = true;
                
                if (inSequence)
                {   
                    AddInfo(tag, null);                     
                    continue;
                }

                string s;
                switch (tag)
                {
                    case (int)(TRANSFER_SYNTAX_UID):
                        s = GetString(elementLength);
                        AddInfo(tag, s);
                        if (s.IndexOf("1.2.4") > -1 || s.IndexOf("1.2.5") > -1)
                        {
                            compressedImage = true;
                            file.Close();
                            // Return gracefully indicating that this type of 
                            // Transfer Syntax cannot be handled
                            return false;
                        }
                        if (s.IndexOf("1.2.840.10008.1.2.2") >= 0)
                            bigEndianTransferSyntax = true;
                        break;
                    case (int)(NUMBER_OF_FRAMES):
                        s = GetString(elementLength);
                        AddInfo(tag, s);
                        double frames = Convert.ToDouble(s, new CultureInfo("en-US")); // s2d(s);
                        if (frames > 1.0)
                            nImages = (int)frames;
                        break;
                    case (int)(SAMPLES_PER_PIXEL):
                        samplesPerPixel = GetShort();
                        AddInfo(tag, samplesPerPixel);
                        break;
                    case (int)(PHOTOMETRIC_INTERPRETATION):
                        photoInterpretation = GetString(elementLength);
                        AddInfo(tag, photoInterpretation);
                        break;
                    case (int)(PLANAR_CONFIGURATION):
                        planarConfiguration = GetShort();
                        AddInfo(tag, planarConfiguration);
                        break;
                    case (int)(ROWS):
                        height = GetShort();
                        AddInfo(tag, height);
                        break;
                    case (int)(COLUMNS):
                        width = GetShort();
                        AddInfo(tag, width);
                        break;
                    case (int)(PIXEL_SPACING):
                        String scale = GetString(elementLength);
                        GetSpatialScale(scale);
                        AddInfo(tag, scale);
                        break;
                    case (int)(SLICE_THICKNESS):
                    case (int)(SLICE_SPACING):
                        String spacing = GetString(elementLength);
                        pixelDepth = Convert.ToDouble(spacing, new CultureInfo("en-US"));
                        AddInfo(tag, spacing);
                        break;
                    case (int)(BITS_ALLOCATED):
                        bitsAllocated = GetShort();
                        AddInfo(tag, bitsAllocated);
                        break;
                    case (int)(PIXEL_REPRESENTATION):
                        int pixelRepresentation = GetShort();
                        AddInfo(tag, pixelRepresentation);
                        break;
                    case (int)(WINDOW_CENTER):
                        String center = GetString(elementLength);
                        int index = center.IndexOf('\\');
                        if (index != -1) center = center.Substring(index + 1);
                        windowCentre = Convert.ToDouble(center, new CultureInfo("en-US"));
                        AddInfo(tag, center);
                        break;
                    case (int)(WINDOW_WIDTH):
                        String widthS = GetString(elementLength);
                        index = widthS.IndexOf('\\');
                        if (index != -1) widthS = widthS.Substring(index + 1);
                        windowWidth = Convert.ToDouble(widthS, new CultureInfo("en-US"));
                        AddInfo(tag, widthS);
                        break;
                    case (int)(RESCALE_INTERCEPT):
                        String intercept = GetString(elementLength);
                        rescaleIntercept = Convert.ToDouble(intercept, new CultureInfo("en-US"));
                        AddInfo(tag, intercept);
                        break;
                    case (int)(RESCALE_SLOPE):
                        String slop = GetString(elementLength);
                        rescaleSlope = Convert.ToDouble(slop, new CultureInfo("en-US"));
                        AddInfo(tag, slop);
                        break;
                    case (int)(RED_PALETTE):
                        reds = GetLut(elementLength);
                        AddInfo(tag, elementLength / 2);
                        break;
                    case (int)(GREEN_PALETTE):
                        greens = GetLut(elementLength);
                        AddInfo(tag, elementLength / 2);
                        break;
                    case (int)(BLUE_PALETTE):
                        blues = GetLut(elementLength);
                        AddInfo(tag, elementLength / 2);
                        break;
                    case (int)(PIXEL_DATA):
                        // Start of image data...
                        if (elementLength != 0)
                        {
                            offset = location;
                            AddInfo(tag, location);
                            decodingTags = false;
                        }
                        else
                            AddInfo(tag, null);
                        break;
                    default:
                        AddInfo(tag, null);
                        break;
                }
            }
            return true;
        }

        void ReadPixels()
        {
            if (bitsAllocated == 8)
            {
                if (pixels8 != null)
                    pixels8.Clear();
                pixels8 = new List<byte>();
                int numPixels = width * height;
                byte[] buf = new byte[numPixels];
                file.BaseStream.Position = offset;
                file.Read(buf, 0, numPixels);

                for (int i = 0; i < numPixels; ++i)
                {
                    pixels8.Add(buf[i]);
                }
            }

            if (bitsAllocated == 16)
            {
                if (pixels16 != null)
                    pixels16.Clear();
                pixels16 = new List<ushort>();
                int numPixels = width * height;
                byte[] bufByte = new byte[numPixels * 2];
                file.BaseStream.Position = offset;
                file.Read(bufByte, 0, numPixels * 2);
                ushort s;
                int i, i1;
                byte b0, b1;

                for (i = 0; i < numPixels; ++i)
                {
                    i1 = i * 2;
                    b0 = bufByte[i1];
                    b1 = bufByte[i1 + 1];
                    s = Convert.ToUInt16((b1 << 8) + b0);
                    pixels16.Add(s);
                }
            }
        }
    }
}*/

#endif