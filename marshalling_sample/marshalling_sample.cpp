#include <string>
#include <tuple>

template <int... X_>
struct sequence {};

template <int N_, int... Y_>
struct sequence_generator : sequence_generator<N_ - 1, N_ - 1, Y_...> {};

template <int... Y_>
struct sequence_generator<0, Y_...> {
  using type = sequence<Y_...>;
};

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

int main()
{
  return 0;
}

