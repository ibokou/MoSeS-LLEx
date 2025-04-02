import 'package:app/pages/home/devices/add_new/sub_pages/page_device_type.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

class NearbyDevicePage extends ConsumerWidget {
  const NearbyDevicePage({super.key});

  @override
  Widget build(BuildContext, WidgetRef ref) {
    final counter = ref.watch(arrowButtonIndexProvider);
    return Scaffold(
        body: Center(
            child: Text(
                '$counter')
        )
    );
  }
}