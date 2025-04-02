abstract class BaseException implements Exception {
  const BaseException({this.message});
  final String? message;

  getClassName() => String;

  @override
  String toString() {
    if(message is String) {
      return '$getClassName(): $message';
    }
    return '$getClassName():';
  }
}