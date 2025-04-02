import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:riverpod_annotation/riverpod_annotation.dart';

part 'page_device_type.g.dart';

class DeviceTypePage extends ConsumerWidget {
  const DeviceTypePage({super.key});

  static const List<Widget> _selections = [
    DeviceSelection(
        assetPath: "assets/home/devices/central_device.jpg",
        title: "Central Device"),
    DeviceSelection(
        assetPath: "assets/home/devices/external_trigger.jpg",
        title: "External Trigger")
  ];

  @override
  Widget build(BuildContext context, WidgetRef ref) => Scaffold(
          body: Center(
              child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          ArrowButton(orientation: ArrowButtonOrientation.left),
          _selections[ref.watch(arrowButtonIndexProvider)],
          ArrowButton(orientation: ArrowButtonOrientation.right)
        ],
      )));
}

@riverpod
class ArrowButtonIndex extends _$ArrowButtonIndex {
  @override
  int build() => 0;

  void set(int newIndex) => {state = newIndex};
}

enum ArrowButtonOrientation { left, right }

class ArrowButton extends ConsumerWidget {
  const ArrowButton({super.key, required this.orientation});

  static const double _splashRadius = 16.0;
  static const double _iconSize = 48;

  final ArrowButtonOrientation orientation;

  @override
  Widget build(BuildContext context, WidgetRef ref) => IconButton(
        splashRadius: _splashRadius,
        padding: EdgeInsets.zero,
        onPressed: () => {
          if (orientation == ArrowButtonOrientation.left)
            {ref.read(arrowButtonIndexProvider.notifier).set(0)}
          else
            {ref.read(arrowButtonIndexProvider.notifier).set(1)}
        },
        icon: Icon(
          orientation == ArrowButtonOrientation.left
              ? Icons.arrow_left_rounded
              : Icons.arrow_right_rounded,
          size: _iconSize,
        ),
      );
}

class DeviceSelection extends StatelessWidget {
  const DeviceSelection(
      {super.key, required this.assetPath, required this.title});

  final String assetPath;

  final String title;

  @override
  Widget build(BuildContext context) => Expanded(
          child: Column(mainAxisAlignment: MainAxisAlignment.center, children: <Widget>[
        Image.asset(assetPath, fit: BoxFit.scaleDown),
        Row(mainAxisAlignment: MainAxisAlignment.center, children:
        [Padding(
          padding: const EdgeInsets.all(12),
          child: Text(title, style: TextStyle(fontSize: 18, fontWeight: FontWeight.w700)),
        ),
        ])
      ]));
}
