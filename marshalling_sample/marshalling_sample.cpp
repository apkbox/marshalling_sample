#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

template <int... X_>
struct sequence {};

template <int N_, int... Y_>
struct sequence_generator : sequence_generator<N_ - 1, N_ - 1, Y_...> {};

template <int... Y_>
struct sequence_generator<0, Y_...> {
  using type = sequence<Y_...>;
};

struct EmptyBase {};

template<typename Base_ = EmptyBase>
struct BasicSerializerT : public Base_ {
  template<typename T_>
  void SerializeValue(const T_ &v, std::string *out) {
    std::ostringstream ss;
    ss << v << std::endl;
    out->append(ss.str());
  }
};

template <typename Tuple_, typename Serializer_, int N_, int... Y_>
struct DataPackerBase : DataPackerBase<Tuple_, Serializer_, N_ - 1, N_ - 1, Y_...> {
  explicit DataPackerBase(Serializer_ &s) : DataPackerBase<Tuple_, Serializer_, N_ - 1, N_ - 1, Y_...>(s)
  {
  }

  virtual void Serialize(const Tuple_ &a, std::string *out) {
    DataPackerBase<Tuple_, Serializer_, N_ - 1, N_ - 1, Y_...>::Serialize(a, out);
    serializer_.SerializeValue(std::get<N_ - 1>(a), out);
  }
};

template <typename Tuple_, typename Serializer_, int... Y_>
struct DataPackerBase<Tuple_, Serializer_, 0, Y_...> {
  explicit DataPackerBase(Serializer_ &s) : serializer_(s)
  {
  }

  virtual void Serialize(const Tuple_ &a, std::string *out) {
    // std::cout << "last" << std::endl;
  }

  Serializer_ serializer_;
};

template<typename Tuple_, typename Serializer_ = BasicSerializer>
std::string SerializeIntoString(const Tuple_ &data, Serializer_ &serializer = BasicSerializerT()) {
  std::string s;
  DataPackerBase<Tuple_, Serializer_, std::tuple_size<Tuple_>::value> x(serializer);
  x.Serialize(data, &s);
  return s;
}

struct EventData {
  int data1;
  const char *string1;
};

class EventSourceInterface {
public:
  virtual ~EventSourceInterface() {};

  virtual void VoidMethod() = 0;
  virtual void IntMethod(int value) = 0;
  virtual void IntIntMethod(int value1, int value2) = 0;
  virtual void IntIntMethod2(int value1, int value2) = 0;
  virtual void IntPtrMethod(int size, EventData *data) = 0;
};


// template<typename Dispath_, typename Params_>
class Marshaller {
public:
  template<typename... Args_>
  static std::string Serialize(Args_... values) {
    return Serialize(std::make_tuple(values...));
  }

  template<typename ArgTuple_>
  static std::string Serialize(const ArgTuple_ &tuple) {
    std::string data;
    // SerializeTo(std::get<0>(tuple), &data));
    return data;
  }
};


class EventSource : public EventSourceInterface {
public:
  void VoidMethod() override {
    MarshallCall<1>(Marshaller::Serialize());
  }

  void IntMethod(int value) override {
    MarshallCall<2>(Marshaller::Serialize(value));
  }

  void IntIntMethod(int value1, int value2) override {
    MarshallCall<3>(Marshaller::Serialize(value1, value2));
  }

  void IntIntMethod2(int value1, int value2) override {
    MarshallCall<4>(Marshaller::Serialize(value1, value2));
  }

  void IntPtrMethod(int size, EventData *data) override {
    MarshallCall<5>(Marshaller::Serialize(size, data));
  }

private:
  template<int Id_>
  void MarshallCall(std::string &serialized_data) {

  }
};

using EventDataPtr = EventData *;

struct MySerializer {
#if 0
  template<typename T_>
  void SerializeValue(const T_ &v, std::string *out) {
    std::ostringstream ss;
    ss << v << std::endl;
    out->append(ss.str());
  }
#endif

  void SerializeValue(const EventDataPtr &v, std::string *out) {
    std::ostringstream s;
    s << v->data1 << "," << v->string1 << std::endl;
    out->append(s.str());
  }
};
#if 0
  template<>
void SerializeValue<EventData *>(EventData * const  &v, std::string *out) {
  std::ostringstream s;
  s << v->data1 << "," << v->string1 << std::endl;
  out->append(s.str());
}
#endif

int main()
{
  //std::tuple<int, long, std::string, double> t = std::make_tuple(1, 43, "aaaaa", 3.14);

  //DataPacker<decltype(t), std::tuple_size<decltype(t)>::value> packer;
  //packer.Serialize(t);

  EventData d;
  d.data1 = 43;
  d.string1 = "Test string";

  BasicSerializerT<MySerializer> ms;
  std::cout << "-- " << SerializeIntoString(std::make_tuple(), ms) << std::endl;
  std::cout << "-- " << SerializeIntoString(std::make_tuple(1), ms) << std::endl;
  std::cout << "-- " << SerializeIntoString(std::make_tuple(1, 2), ms) << std::endl;
  std::cout << "-- " << SerializeIntoString(std::make_tuple(1, "test"), ms) << std::endl;
  std::cout << "-- " << SerializeIntoString(std::make_tuple(1, &d), ms) << std::endl;

  return 0;
}

