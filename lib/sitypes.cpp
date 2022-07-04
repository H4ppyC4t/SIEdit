#include "sitypes.h"

#include "chunk.h"

namespace si {

Data ReadU32(std::ifstream &is)
{
  u32 u;
  is.read((char *) &u, sizeof(u));
  return u;
}

Data ReadU16(std::ifstream &is)
{
  u16 u;
  is.read((char *) &u, sizeof(u));
  return u;
}

Data ReadString(std::ifstream &is)
{
  bytearray d;

  while (true) {
    char c;
    is.read(&c, 1);
    if (c == 0) {
      break;
    }
    d.push_back(c);
  }

  // Append null terminator
  d.push_back(0);

  return d;
}

Data ReadBytes(std::ifstream &is, size_t size)
{
  bytearray d;

  d.resize(size);
  is.read(d.data(), size);

  return d;
}

void RIFF::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  data["Format"] = ReadU32(is);
}

void LIST::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  data["Format"] = ReadU32(is);

  if (data["Format"] == Chunk::TYPE_MxCh) {
    data["Count"] = ReadU32(is);
  }
}

void MxSt::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  // MxSt is a container type only and has no members, so nothing needs to be done here
}

void MxHd::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  Data v = ReadU32(is);
  data["Version"] = v;
  data["BufferSize"] = ReadU32(is);
  data["BufferCount"] = ReadU32(is);
}

void MxCh::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  data["Flags"] = ReadU16(is);
  data["Object"] = ReadU32(is);
  data["Time"] = ReadU32(is);
  data["DataSize"] = ReadU32(is);
  data["Data"] = ReadBytes(is, size - 0xE);
}

void MxOf::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  data["Count"] = ReadU32(is);
  data["Offsets"] = ReadBytes(is, size - sizeof(u32));
}

void pad_::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  is.seekg(size, std::ios::cur);
}

const char *MxOb::GetTypeName(Type type)
{
  switch (type) {
  case SMK:
    return "SMK";
  case WAV:
    return "WAV";
  case Presenter:
    return "MxPresenter";
  case BMP:
    return "BMP";
  case OBJ:
    return "3D Object";
  case World:
    return "World";
  case Event:
    return "Event";
  case Animation:
    return "Animation";
  case TYPE_COUNT:
    break;
  }

  return "Unknown";
}

void MxOb::Read(std::ifstream &is, DataMap &data, u32 version, u32 size)
{
  Data obj_type = ReadU16(is);
  data["Type"] = obj_type;
  data["Presenter"] = ReadString(is);
  data["Unknown1"] = ReadU32(is);
  data["Name"] = ReadString(is);
  data["ID"] = ReadU32(is);
  data["Unknown3"] = ReadU32(is);
  data["Unknown4"] = ReadU32(is);
  data["Unknown5"] = ReadU32(is);
  data["Unknown6"] = ReadU32(is);
  data["Unknown7"] = ReadU32(is);
  data["Unknown8"] = ReadU32(is);
  data["Unknown9"] = ReadU32(is);
  data["Unknown10"] = ReadU32(is);
  data["Unknown11"] = ReadU32(is);
  data["Unknown12"] = ReadU32(is);
  data["Unknown13"] = ReadU32(is);
  data["Unknown14"] = ReadU32(is);
  data["Unknown15"] = ReadU32(is);
  data["Unknown16"] = ReadU32(is);
  data["Unknown17"] = ReadU32(is);
  data["Unknown18"] = ReadU32(is);
  data["Unknown19"] = ReadU32(is);
  data["Unknown20"] = ReadU32(is);
  data["Unknown21"] = ReadU32(is);
  data["Unknown22"] = ReadU32(is);
  data["Unknown23"] = ReadU32(is);
  data["Unknown24"] = ReadU32(is);

  Data extra_sz = ReadU16(is);
  data["ExtraLength"] = extra_sz;
  data["ExtraData"] = ReadBytes(is, extra_sz);

  if (obj_type != Presenter && obj_type != World) {
    data["FileName"] = ReadString(is);

    data["Unknown26"] = ReadU32(is);
    data["Unknown27"] = ReadU32(is);
    data["Unknown28"] = ReadU32(is);

    data["FileType"] = ReadU32(is);

    data["Unknown29"] = ReadU32(is);
    data["Unknown30"] = ReadU32(is);

    if (obj_type == MxOb::WAV) {
      data["Unknown31"] = ReadU32(is);
    }
  }
}

}