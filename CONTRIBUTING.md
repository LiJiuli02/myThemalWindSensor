# Contributing / 贡献指南

[English overview](README.md) · [中文概览](README.zh-CN.md)

## Scope / 范围

**EN:** This repository preserves a course project. Improvements should make its implementation and reproduction easier to understand, while distinguishing historical experiments, current behavior and proposed work. Start with [architecture](docs/ARCHITECTURE.md) and [validation](docs/VALIDATION.md).

**中文：** 本仓库保留课程项目，改进应帮助读者理解实现和复现过程，并区分历史实验、当前行为及计划工作。请先阅读架构与验证文档。

## Changes / 修改要求

1. **EN:** Keep `README.md` and `README.zh-CN.md` equivalent. Other project guides contain paired English and Chinese text; update both when behavior changes.
   **中文：** 保持两版 README 内容一致，其他指南的中英文内容需同步更新。
2. **EN:** Keep paths, identifiers, units, equations and example values consistent across languages. Explain specialist terms at first use.
   **中文：** 两种语言的路径、标识符、单位、公式与示例值应一致，专业术语首次出现时加以说明。
3. **EN:** Anchor hardware, tool versions and performance claims to source, configuration or dated measurements. Label calculations, examples and proposed tests explicitly.
   **中文：** 硬件、版本与性能结论应有源码、配置或带日期的测量依据，并标明推导、示例和建议测试。
4. **EN:** Preserve existing source and third-party attribution. Avoid including generated caches, personal machine paths, credentials or unrelated project changes in new commits.
   **中文：** 保留源码与第三方来源说明，新的提交中避免缓存、个人机器路径、凭据及无关修改。
5. **EN:** Separate firmware/RTL fixes from documentation corrections when they require different validation.
   **中文：** 固件或 RTL 修复与文档修正若需不同验证，应分别提交。

## Reporting an issue / 报告问题

**EN:** Include the commit, hardware revision, tool versions, steps, expected behavior and observed behavior. Attach minimal logs or data with sensitive values removed. State whether the issue was reproduced on hardware, in simulation or by source inspection.

**中文：** 提供提交、硬件版本、工具版本、步骤、预期与实际表现；附最少必要且脱敏的日志或数据，注明属于实物复现、仿真还是源码核查。

## Review checklist / 检查清单

- [ ] English and Chinese remain consistent. / 中英文一致。
- [ ] Relative links and filenames resolve with exact capitalization. / 相对链接及文件名大小写正确。
- [ ] Commands name their working directory and prerequisites. / 命令注明工作目录和前提。
- [ ] Results are distinguished from unexecuted plans. / 区分已得结果与未执行计划。
- [ ] Relevant validation evidence is included, or the limitation is stated. / 提供相应验证证据，或说明限制。
- [ ] Existing attribution and licensing notices remain intact. / 保留现有署名与许可声明。

**EN:** No repository-level license was found in the documented baseline. This guide does not introduce a license or change existing third-party terms.

**中文：** 文档基准版本未发现仓库级许可证；本指南不引入新许可或改变第三方条款。
