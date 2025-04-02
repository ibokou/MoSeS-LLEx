import 'package:riverpod_annotation/riverpod_annotation.dart';

part 'selection.g.dart';

enum SubPageId { deviceType, device, numSubPages }

@riverpod
class SubPageSelection extends _$SubPageSelection {
  @override
  SubPageId build() => SubPageId.deviceType;

  void set(SubPageId newSubPageId) => {state = newSubPageId};
}
