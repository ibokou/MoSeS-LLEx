import 'package:app/pages/home/devices/add_new/sub_pages/selection.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class SegmentedButtonSubPageSelection extends ConsumerWidget {
  const SegmentedButtonSubPageSelection({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) =>
      SegmentedButton<SubPageId>(
        showSelectedIcon: false,
        style: SegmentedButton.styleFrom(
          backgroundColor: Colors.white,
          foregroundColor: Colors.grey,
          selectedForegroundColor: Colors.white,
          selectedBackgroundColor: Colors.blueAccent,
        ),
        segments: const <ButtonSegment<SubPageId>>[
          ButtonSegment<SubPageId>(
              value: SubPageId.deviceType,
              label: Text('Device Type'),
              icon: Icon(Icons.developer_board)),
          ButtonSegment<SubPageId>(
              value: SubPageId.device,
              label: Text('Nearby Device'),
              icon: Icon(Icons.bluetooth_searching_rounded)),
        ],
        selected: <SubPageId>{ref.watch(subPageSelectionProvider)},
        onSelectionChanged: (Set<SubPageId> newSetting) => {
          ref.read(subPageSelectionProvider.notifier).set(newSetting.first),
        },
      );
}
